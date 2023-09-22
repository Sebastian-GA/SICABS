/**
 * @file sleep.cpp
 * @brief Sleep functions
 */

#include "sleep.h"

/**
 * @brief Setup sleep
 */
void setupSleep() {
    // ============== PIR SENSOR ==============
    pinMode(PIR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionDetected, CHANGE);  // Reset the timerToSleep

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
}

/**
 * @brief Go to sleep
 * @param xTimer
 *        Timer handle
 */
void goToSleep(TimerHandle_t xTimer) {
    Serial.println("Going to sleep");
    Serial.flush();

    fingerprintSensor.disable();

    esp_deep_sleep_start();
}

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