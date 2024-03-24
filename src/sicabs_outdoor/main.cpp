#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
bool sendOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
bool sendFakeOpen = false;

// Timer stuff
uint8_t PIRSensorPIN = 15;
static TimerHandle_t timerToSleep = NULL;

// when timer is over
void whenTimerIsOver(TimerHandle_t xTimer);

// when there's a change in movement
void IRAM_ATTR motionDetected();

volatile bool oledOn = true;
volatile bool sendImagesOn = true;

void setup() {
    Serial.begin(115200);
    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 10000, NULL, 1, NULL, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    // ============== TIMER ===================
    timerToSleep = xTimerCreate(
        "Sleep timer",
        10000 / portTICK_PERIOD_MS,
        pdFALSE,
        (void *)0,
        whenTimerIsOver);
    xTimerStart(timerToSleep, portMAX_DELAY);

    // // =================== PIR SENSOR ===============
    pinMode(PIRSensorPIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PIRSensorPIN), motionDetected, CHANGE);

    // Serial.println("\n\n\n");
    // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    vTaskDelete(NULL);
}
void loop() {
}

void whenTimerIsOver(TimerHandle_t xTimer) {
    Serial.println("went to sleep for now................");
    oledOn = false;
    sendImagesOn = false;
}

void IRAM_ATTR motionDetected() {
    if (digitalRead(PIRSensorPIN) == HIGH) {
        xTimerStopFromISR(timerToSleep, NULL);
        oledOn = true;
        sendImagesOn = true;
    } else {
        xTimerResetFromISR(timerToSleep, NULL);
    }
}