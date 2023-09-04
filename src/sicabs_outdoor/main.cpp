#include <Arduino.h>
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <Wire.h>

#include <sfm.hpp>

#include "common.h"
#include "config.h"
#include "credentials.h"

// Globals
static TimerHandle_t timerToSleep = NULL;  // Timer to go to sleep
// Keypad_I2C keypad = Keypad_I2C(makeKeymap(KEYPAD_KEYS), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_ADDR);
SFM_Module fingerprintSensor = SFM_Module(SFM_VCC, SFM_IRQ, SFM_TX, SFM_RX);
volatile bool fingerTouchFlag = false;

// ISR
void goToSleep(TimerHandle_t xTimer) {
    Serial.println("Going to sleep");
    Serial.flush();

    fingerprintSensor.disable();
    esp_deep_sleep_start();
}

void IRAM_ATTR motionDetected() {
    if (digitalRead(BTN_BUILTIN) == LOW) {  // If motion detected
        xTimerStopFromISR(timerToSleep, NULL);
    } else {  // If motion stopped
        xTimerResetFromISR(timerToSleep, NULL);
    }
}

void IRAM_ATTR fingerprintInterrupt() {
    volatile bool fingerTouchLastState = fingerprintSensor.isTouched();
    fingerprintSensor.pinInterrupt();  // Update fingerprintSensor.touched variable

    if (!fingerTouchFlag) {
        fingerTouchFlag = fingerTouchLastState != fingerprintSensor.isTouched();
    }

    motionDetected();  // Reset the timerToSleep
}

// Tasks
// void updateKeypad(void *parameters) {
//     while (true) {
//         // TODO: Check if necessary to add a mutex
//         keypad.getKeys();  // If a key changed state, it will call the
//                            // keypadEvent() function and return true

//         // Delay task for half the time as the KEYPAD_DEBOUNCE_TIME_MS
//         // to prevent the task from running too often
//         vTaskDelay((KEYPAD_DEBOUNCE_TIME_MS / 2) / portTICK_PERIOD_MS);
//     }
// }

// void keypadEvent(KeypadEvent key) {
//     KeyState keyState = keypad.key[keypad.findInList(key)].kstate;
//     // keypad.getState() returns only the state of the key in first position
//     // of the key list, that's the first key pressed

//     switch (keyState) {
//         case PRESSED:
//             switch (key) {
//                 case '*':
//                     break;
//             }
//             break;
//         case RELEASED:
//             switch (key) {
//                 case '*':
//                     break;
//             }
//             break;
//         case HOLD:
//             switch (key) {
//                 case '*':
//                     break;
//             }
//             break;
//         case IDLE:
//             // In this state, this function is never called
//             break;
//     }
// }

// Setup and loop
void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Outdoor---");

    // ============== PIR SENSOR ==============
    pinMode(PIR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionDetected, CHANGE);  // Reset the timerToSleep

    // ============== KEYPAD ==============
    // Initialize keypad
    // Wire.begin();
    // keypad.begin();
    // keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME_MS);
    // keypad.setHoldTime(KEYPAD_HOLD_TIME_MS);
    // keypad.addEventListener(keypadEvent);
    // // Create keypad task
    // xTaskCreatePinnedToCore(
    //     updateKeypad,    // Function to be called
    //     "Update Keypad",  // Name of the task
    //     10000,            // Stack size in bytes  // TODO: Verify this value
    //     NULL,             // Task input parameter
    //     1,                // Priority of the task
    //     NULL,             // Task handle
    //     0);               // Core where the task should run (0 or 1)

    // ============== FINGERPRINT SENSOR ==============
    fingerprintSensor.setPinInterrupt(fingerprintInterrupt);
    fingerprintSensor.setRingColor(SFM_RING_YELLOW, SFM_RING_OFF);

    // ============== DEEP SLEEP ==============
    // Timer to go to sleep
    timerToSleep = xTimerCreate("Sleep Timer",                        // Name
                                SLEEP_DELAY_MS / portTICK_PERIOD_MS,  // Timer ticks before expiration (max resolution is 1 ms)
                                pdFALSE,                              // Don't auto-reload
                                (void *)0,                            // Timer ID
                                goToSleep);                           // Callback function
    // Check if timer was created
    if (timerToSleep == NULL) {
        Serial.println("Timer creation failed");
    } else {
        // Start the timer
        Serial.println("Starting timer");
        xTimerStart(timerToSleep, portMAX_DELAY);
    }
    // Set wakeup sources
    esp_sleep_enable_ext1_wakeup(WAKEUP_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

    // Delete setup and loop tasks
    // vTaskDelete(NULL);
}

void loop() {
    if (fingerTouchFlag) {
        fingerTouchFlag = false;
        if (fingerprintSensor.isTouched()) {
            Serial.println("Fingerprint sensor touched");
            // fingerprintSensor.enable();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            fingerprintSensor.setRingColor(SFM_RING_GREEN, SFM_RING_OFF);

        } else {
            fingerprintSensor.setRingColor(SFM_RING_RED, SFM_RING_OFF);
            vTaskDelay(200 / portTICK_PERIOD_MS);
            // fingerprintSensor.disable();
        }
    }
}
