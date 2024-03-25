#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
bool sendOpen = false;
bool sendFakeOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

// For controlling idle state
bool idle = false;
SemaphoreHandle_t idleStateMutex = xSemaphoreCreateMutex();

void setup() {
    Serial.begin(115200);
    // xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(PIRSensorSleepControl, "PIR sensor control", 2000, NULL, 1, NULL, 0);

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    vTaskDelete(NULL);
}
void loop() {
}