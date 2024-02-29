#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"
// void connectToWiFi();
// void wifiEvent(WiFiEvent_t event);

extern bool sendOpen;
extern SemaphoreHandle_t mutex;

Camera camera;

void videoTransmission(void* parameter) {
    Serial.begin(115200);
    int localVar;

    // unsigned long int actual = millis();
    // unsigned long int interval = 6000;

    camera.initCamera();
    camera.connectToWifi();
    camera.connectClient();

    while (1) {
        camera.sendImageToIndoor();
        // if (millis() >= actual + interval) {
        //     camera.sendTextToIndoor();
        //     actual = millis();
        // }

        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            // Do something
            if (sendOpen) {
                camera.sendTextToIndoor();
                sendOpen = false;
            }
            xSemaphoreGive(mutex);
        } else {
        }
    }
}