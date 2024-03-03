#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"
#include "./MemoryManager/MemoryManager.h"

extern bool sendOpen;
extern SemaphoreHandle_t mutex;

Camera camera;
MemoryManager memoryManager;

void videoTransmission(void* parameter) {
    Serial.begin(115200);

    camera.initCamera();
    camera.connectToWifi();
    camera.connectClient();
    // String sampleString = memoryManager.readString();

    while (1) {
        // Serial.println(sampleString);
        // delay(800);
        camera.sendImageToIndoor();

        // if (xSemaphoreTake(mutex, 0) == pdTRUE) {
        //     // Do something
        //     if (sendOpen) {
        //         camera.sendTextToIndoor();
        //         sendOpen = false;
        //     }
        //     xSemaphoreGive(mutex);
        // } else {
        // }
    }
}