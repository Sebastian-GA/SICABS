#include <Arduino.h>

#include "./Tasks/Tasks.hpp"

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);

    vTaskDelete(NULL);
}

void loop() {
}