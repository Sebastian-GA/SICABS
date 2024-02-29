#include <Arduino.h>

#include "tasks.hpp"

// TaskHandle_t led;
TaskHandle_t video;

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Indoor---");
    Serial.println();

    if (xTaskCreatePinnedToCore(videoProjection, "Video projection", 100000, NULL, 2, &video, 1) != pdPASS) {
        Serial.println("video not allocated :(");
    }

    // if (xTaskCreatePinnedToCore(blinkLED, "blink LED", 100000, NULL, 1, &led, 0) != pdPASS) {
    //     Serial.println("LED blinking not allocated :(");
    // }

    vTaskDelete(NULL);
}

void loop() {
}
