#include <Arduino.h>

#define SICABS_OUTDOOR
#include "config.h"
#include "credentials.h"

// Globals
static TimerHandle_t timer_sleep = NULL;  // Timer to go to sleep

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

// Setup
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

    // Delete setup and loop tasks
    vTaskDelete(NULL);
}

void loop() {}