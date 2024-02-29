#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"

extern int sharedVar;
extern SemaphoreHandle_t mutex;

Camera camera;

void videoTransmission(void* parameter) {
    Serial.begin(115200);
    int localVar;

    camera.initCamera();
    camera.connectToWifi();

    while (1) {
        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            xSemaphoreGive(mutex);
        } else {
        }
    }
}