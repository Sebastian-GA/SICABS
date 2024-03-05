#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"
#include "./MemoryManager/MemoryManager.h"

extern bool sendOpen;
extern SemaphoreHandle_t mutex;

Camera camera;
// MemoryManager memoryManager;
void onMessageCallback(websockets::WebsocketsMessage message);
bool sendImageFlag = true;
int toIncrement = 0;

void videoTransmission(void* parameter) {
    Serial.begin(115200);

    camera.initCamera();
    camera.connectToWifi();
    camera.connectClient();
    camera.client.onMessage(onMessageCallback);
    // 1) Set variable to be incremented to zero

    while (1) {
        // Await for messages
        if (camera.client.available()) {
            camera.client.poll();
        }

        // Check if it's time to send the message
        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            if (sendOpen) {
                // Stop camera from sending images for a while
                sendImageFlag = false;
                // Send open message instead
                camera.client.send(String(toIncrement));
                Serial.print("Number sent from camera: ");
                Serial.println(toIncrement);

                sendOpen = false;
            }
            xSemaphoreGive(mutex);
        }
        // Send images if allowed
        if (sendImageFlag)
            camera.sendImageToIndoor();
    }
}

String localMessage;
void onMessageCallback(websockets::WebsocketsMessage message) {
    Serial.print("received a message! ");
    // Allows images to be sent as long as there's a response
    sendImageFlag = true;
    if (message.isText()) {
        // Save it locally in a string
        localMessage = message.c_str();
        // Parse it to int and check if it's the same number incremented by 1
        if (localMessage.toInt() == toIncrement + 1) {
            Serial.println("Indeed it's the expected number :)");
            toIncrement = toIncrement + 2;
        }
        // In case it's not, then send error message to console
        else {
            Serial.println("Not the expected number :(");
        }

        Serial.print("the esp32 cam received a message: ");
        Serial.println(message.c_str());
    }
}