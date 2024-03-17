#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
bool sendOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
bool sendFakeOpen = false;
void setup() {
    Serial.begin(115200);
    Serial.println("\n\n\n");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 10000, NULL, 1, NULL, 1);
    vTaskDelete(NULL);
}
void loop() {
}
