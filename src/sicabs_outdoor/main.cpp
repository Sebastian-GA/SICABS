#include <Arduino.h>

#include "credentials.h"
#include "pin_definitions.h"
#include "tasks.hpp"

TaskHandle_t video;

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Test---");
    Serial.println();

    if (xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 100000, NULL, 2, &video, 1) != pdPASS) {
        Serial.println("video not allocated :(");
    }

    vTaskDelete(NULL);
}

void loop() {
    vTaskDelete(NULL);
}
