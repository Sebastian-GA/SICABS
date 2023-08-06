#include <Arduino.h>
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <Wire.h>

#include "common.h"
#include "config.h"
#include "credentials.h"

// Globals
static TimerHandle_t timer_sleep = NULL;  // Timer to go to sleep
Keypad_I2C keypad = Keypad_I2C(makeKeymap(KEYPAD_KEYS), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_ADDR);

// ISR
void go_to_sleep(TimerHandle_t xTimer) {
    Serial.println("Going to sleep");
    Serial.flush();
    esp_deep_sleep_start();
}

void IRAM_ATTR motion_detected() {
    if (digitalRead(BTN_BUILTIN) == LOW) {  // If motion detected
        xTimerStopFromISR(timer_sleep, NULL);
    } else {  // If motion stopped
        xTimerResetFromISR(timer_sleep, NULL);
    }
}

// Tasks
void update_keypad(void *parameters) {
    while (true) {
        // TODO: Check if necessary to add a mutex
        keypad.getKeys();  // If a key changed state, it will call the
                           // keypadEvent() function and return true

        // Delay task for half the time as the KEYPAD_DEBOUNCE_TIME_MS
        // to prevent the task from running too often
        vTaskDelay((KEYPAD_DEBOUNCE_TIME_MS / 2) / portTICK_PERIOD_MS);
    }
}

void keypadEvent(KeypadEvent key) {
    KeyState keyState = keypad.key[keypad.findInList(key)].kstate;
    // keypad.getState() returns only the state of the key in first position
    // of the key list, that's the first key pressed

    switch (keyState) {
        case PRESSED:
            switch (key) {
                case '*':
                    break;
            }
            break;
        case RELEASED:
            switch (key) {
                case '*':
                    break;
            }
            break;
        case HOLD:
            switch (key) {
                case '*':
                    break;
            }
            break;
        case IDLE:
            // In this state, this function is never called
            break;
    }
}

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Outdoor---");

    // ============== DEEP SLEEP ==============
    // Configure wake up pin
    pinMode(BTN_BUILTIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BTN_BUILTIN), motion_detected, CHANGE);  // Reset the timer_sleep
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);                                   // Enable wake up for motion detection

    // Timer to go to sleep
    timer_sleep = xTimerCreate("Sleep Timer",                        // Name
                               SLEEP_DELAY_MS / portTICK_PERIOD_MS,  // Timer ticks before expiration (max resolution is 1 ms)
                               pdFALSE,                              // Don't auto-reload
                               (void *)0,                            // Timer ID
                               go_to_sleep);                         // Callback function
    // Check if timer was created
    if (timer_sleep == NULL) {
        Serial.println("Timer creation failed");
    } else {
        // Start the timer
        Serial.println("Starting timer");
        xTimerStart(timer_sleep, portMAX_DELAY);
    }

    // ============== KEYPAD ==============
    // Initialize keypad
    Wire.begin();
    keypad.begin();
    keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME_MS);
    keypad.setHoldTime(KEYPAD_HOLD_TIME_MS);
    keypad.addEventListener(keypadEvent);
    // Create keypad task
    xTaskCreatePinnedToCore(
        update_keypad,    // Function to be called
        "Update Keypad",  // Name of the task
        10000,            // Stack size in bytes  // TODO: Verify this value
        NULL,             // Task input parameter
        1,                // Priority of the task
        NULL,             // Task handle
        0);               // Core where the task should run (0 or 1)

    // Delete setup and loop tasks
    vTaskDelete(NULL);
}

void loop() {}
