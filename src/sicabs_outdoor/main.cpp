#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
bool sendOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
bool sendFakeOpen = false;
void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 4000, NULL, 1, NULL, 1);
    vTaskDelete(NULL);
}
void loop() {
}