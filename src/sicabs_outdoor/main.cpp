/**
 * @file main.cpp
 * @brief Main file
 */

/**************************************************************************
 *  INCLUDES
 **************************************************************************/

#include <Arduino.h>
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <WiFi.h>
#include <Wire.h>

#include <sfm.hpp>

#include "esp_now.h"

/**************************************************************************
 * DEFINES
 **************************************************************************/

#include "common.h"
#include "config.h"
#include "credentials.h"
#include "pin_definitions.h"

/**************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************/

TwoWire WireI2C = TwoWire(0);

Keypad_I2C keypad = Keypad_I2C(makeKeymap(KEYPAD_KEYS), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_ADDR, 1, &WireI2C);
static QueueHandle_t keypadQueue = NULL;  // Queue to store keypad input

SFM_Module SFM = SFM_Module(SFM_VCC, SFM_IRQ, SFM_TX, SFM_RX);
uint8_t SFM_return = 0;  // used to get recognition return
uint16_t SFM_uid = 0;    // used to get recognized uid
bool touchState = false;
bool lastTouchState = false;

static TimerHandle_t timerToSleep = NULL;  // Timer to go to sleep

FSMState FSMCurrentState = FSM_IDLE;
FSMState FSMLastState = FSM_IDLE;

char passwordBuffer[PASSWORD_LENGTH];  // Password

/**************************************************************************
 * FUNCTION DECLARATIONS
 **************************************************************************/

// Functions

void goToSleep(TimerHandle_t xTimer);

// Interrupts

void IRAM_ATTR motionDetected();

// Callbacks

void keypadEvent(KeypadEvent key);

// Tasks

void updateKeypad(void *parameters);

/**************************************************************************
 * SETUP AND LOOP
 **************************************************************************/

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Outdoor---");
    Serial.println();

    // ============== PIR SENSOR ==============
    pinMode(PIR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionDetected, CHANGE);  // Reset the timerToSleep

    // ============== DEEP SLEEP ==============
    // Timer to go to sleep
    timerToSleep = xTimerCreate("Sleep Timer",                        // Name
                                SLEEP_DELAY_MS / portTICK_PERIOD_MS,  // Timer ticks before expiration
                                pdFALSE,                              // Don't auto-reload
                                (void *)0,                            // Timer ID
                                goToSleep);                           // Callback function
    // Check if timer was created
    if (timerToSleep == NULL)
        ESP.restart();                         // Timer creation failed
    xTimerStart(timerToSleep, portMAX_DELAY);  // Start the timer

    // Set wakeup sources
    esp_sleep_enable_ext1_wakeup(WAKEUP_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

    // ============== KEYPAD ==============
    // Initialize keypad
    WireI2C.begin(I2C_SDA, I2C_SCL, 100000);
    keypad.begin();
    keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME_MS);
    keypad.setHoldTime(KEYPAD_HOLD_TIME_MS);
    keypad.addEventListener(keypadEvent);
    // Create keypad task
    xTaskCreatePinnedToCore(
        updateKeypad,     // Function to be called
        "Update Keypad",  // Name of the task
        100000,           // Stack size in bytes  // TODO: Verify this value
        NULL,             // Task input parameter
        1,                // Priority of the task
        NULL,             // Task handle
        0);               // Core where the task should run (0 or 1)
    // Create keypad queue
    keypadQueue = xQueueCreate(10, sizeof(char));

    // ============== FINGERPRINT SENSOR ==============
    SFM.setRingColor(SFM_RING_OFF);
    SFM.enable();
    if (SFM.getUuid() != FINGERPRINT_UUID) {  // Validate ID of fingerprint sensor
        Serial.println("Fingerprint sensor not connected or not valid ID");
        ESP.restart();
    }

    // ============== ESP-NOW ==============
    // // Initialize ESP-NOW
    // if (esp_now_init() != ESP_OK) {
    //     Serial.println("Error initializing ESP-NOW");
    //     return;
    // }
    // // Set PMK (Pairwise Master Key)
    // if (esp_now_set_pmk((uint8_t *)ESP_NOW_PMK) != ESP_OK) {
    //     Serial.println("Error setting PMK");
    //     return;
    // }
    // // Register SICABS Indoor as peer
    // esp_now_peer_info_t peerInfo;
    // memcpy(peerInfo.peer_addr, ESP_IN_WIFI_MAC, 6

    FSMCurrentState = FSM_TOCK_TOCK;
}

void loop() {
    switch (FSMCurrentState) {
        // ============== FSM IDLE ==============
        case FSM_IDLE:
            // Do nothing
            break;

        // ============== FSM TOCK TOCK ==============
        case FSM_TOCK_TOCK:
            // After wake up, start streaming video and transmit tock tock signal
            // via ESP-NOW to SICABS Indoor

            // First time in this state
            if (FSMLastState != FSMCurrentState) {
                FSMLastState = FSMCurrentState;
                Serial.println("Tock tock");
            }

            // TODO: Start streaming video
            // TODO: Send tock tock signal via ESP-NOW

            FSMCurrentState = FSM_WAIT_FOR_INPUT;
            break;

        // ============== FSM WAIT FOR INPUT ==============
        case FSM_WAIT_FOR_INPUT:
            // Waiting for input from keypad or fingerprint sensor

            static uint8_t pwdIndex = 0;

            // First time in this state
            if (FSMLastState != FSMCurrentState) {
                FSMLastState = FSMCurrentState;
                SFM.setRingColor(SFM_RING_YELLOW, SFM_RING_OFF);
                xQueueReset(keypadQueue);  // Clear keypad queue
                pwdIndex = 0;              // Reset password index
                lastTouchState = false;    // Reset touch state

                Serial.println("Please put your finger or insert your password");
            }

            // Read from fingerprint sensor
            touchState = SFM.isTouched();
            if (touchState != lastTouchState) {
                lastTouchState = touchState;
                if (touchState) {
                    SFM_return = SFM.recognition_1vN(SFM_uid);
                    if (SFM_return == SFM_ACK_SUCCESS) {
                        if (SFM_uid != 0) {
                            SFM.setRingColor(SFM_RING_GREEN);
                            Serial.printf("Successfully matched with UID: %d", SFM_uid);
                            passwordBuffer[0] = 0;  // Reset password buffer
                            FSMCurrentState = FSM_VALIDATE_PASSWORD;
                        } else {
                            SFM.setRingColor(SFM_RING_RED);
                            Serial.println("Fingerprint not recognized");
                        }
                    } else {
                        Serial.println("ERROR: Restart the Sensor");
                        SFM.disable();
                        delay(100);
                        SFM.enable();
                    }
                } else {
                    delay(1000);
                    SFM.setRingColor(SFM_RING_YELLOW, SFM_RING_OFF);
                }
            }

            // Read from queue the keys pressed
            char key;
            if (xQueueReceive(keypadQueue, &key, 0) == pdTRUE) {
                Serial.print("Key pressed: ");
                Serial.println(key);

                if (key != '#' && key != '*') {  // If key is a number
                    // TODO: Print on Screen *
                    passwordBuffer[pwdIndex++] = key;
                } else {
                    // TODO: Print some temporary character on Screen
                    SFM.setRingColor(SFM_RING_PURPLE, SFM_RING_OFF);
                    delay(1000);
                    SFM.setRingColor(SFM_RING_YELLOW, SFM_RING_OFF);
                }
                if (pwdIndex == PASSWORD_LENGTH) {  // If password is complete
                    FSMCurrentState = FSM_VALIDATE_PASSWORD;
                }
            }

            break;

        // ============== FSM VALIDATE PASSWORD ==============
        case FSM_VALIDATE_PASSWORD:
            // Validate password
            if (FSMLastState != FSMCurrentState) {  // First time in this state
                FSMLastState = FSMCurrentState;
                Serial.println("Validating password");
            }

            Serial.println(passwordBuffer);

            break;

        // ============== FSM REGISTER NEW FINGERPRINT ==============
        case FSM_REGISTER_NEW_FINGERPRINT:
            // Register new fingerprint

            break;

        // ============== FSM REGISTER NEW PASSWORD ==============
        case FSM_REGISTER_NEW_PASSWORD:
            // Register new password

            break;
    }
}

/**************************************************************************
 * FUNCTIONS
 **************************************************************************/

/**
 * @brief Go to sleep
 * @param xTimer
 *        Timer handle
 */
void goToSleep(TimerHandle_t xTimer) {
    Serial.println("Going to sleep");
    Serial.flush();

    SFM.setRingColor(SFM_RING_OFF, SFM_RING_OFF);
    SFM.disable();

    esp_deep_sleep_start();
}

/**************************************************************************
 * INTERRUPTS
 **************************************************************************/

/**
 * @brief Motion detected interrupt
 */
void IRAM_ATTR motionDetected() {
    if (digitalRead(PIR_PIN) == HIGH) {  // If motion detected
        xTimerStopFromISR(timerToSleep, NULL);
    } else {  // If motion stopped
        xTimerResetFromISR(timerToSleep, NULL);
    }
}

/**************************************************************************
 * CALLBACKS
 **************************************************************************/

/**
 * @brief Keypad event
 * @param key
 *        Key pressed
 */
void keypadEvent(KeypadEvent key) {
    KeyState keyState = keypad.key[keypad.findInList(key)].kstate;
    // keypad.getState() returns only the state of the key in first position
    // of the key list, that's the first key pressed

    switch (keyState) {
        case PRESSED:
            if (xQueueSend(keypadQueue, &key, 0) != pdTRUE) {
                Serial.println("Failed to send key to queue");  // Queue full
            }
            break;
        case RELEASED:
            break;
        case HOLD:
            break;
        case IDLE:
            // In this state, this function is called only once after released
            break;
    }
}

/**************************************************************************
 * TASKS
 **************************************************************************/

/**
 * @brief Task to update the keypad state
 * @param parameters
 *        Task parameters
 */
void updateKeypad(void *parameters) {
    while (true) {
        // TODO: Check if necessary to add a mutex
        keypad.getKeys();  // If a key changed state, it will call the
                           // keypadEvent() function and return true

        // Delay task for half the time as the KEYPAD_DEBOUNCE_TIME_MS
        // to prevent the task from running too often
        vTaskDelay((KEYPAD_DEBOUNCE_TIME_MS / 2) / portTICK_PERIOD_MS);
    }
}
