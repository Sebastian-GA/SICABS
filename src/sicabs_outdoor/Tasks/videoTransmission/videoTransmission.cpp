#include <Arduino.h>
#include <math.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"
#include "./MemoryManager/MemoryManager.h"
#include "EncryptionManager.h"
#include "credentials.h"

extern bool sendOpen;
extern SemaphoreHandle_t mutex;

Camera camera;
EncryptionManager encryptionManager;
void onMessageCallback(websockets::WebsocketsMessage message);

bool sendImageFlag = true;
int toIncrement = 0;

void videoTransmission(void* parameter) {
    Serial.begin(115200);
    encryptionManager.encryptionManagerInit(communicationAESKey, communicationIV);

    // camera.initCamera();
    // camera.connectToWifi();
    // camera.connectClient();
    // camera.client.onMessage(onMessageCallback);
    while (1) {
        Serial.print("The incremented number looks like: ");
        Serial.println(toIncrement);
        Serial.print("And encrypted it looks like:");
        String toIncrementEncrypted = encryptionManager.encrypt(toIncrement);
        Serial.println(toIncrementEncrypted);
        Serial.print("Decrypted it looks like:");
        int toIncrementDecrypted = encryptionManager.decrypt(toIncrementEncrypted);
        Serial.println(toIncrementDecrypted);
        Serial.print("Incremented by 1 it looks like: ");
        Serial.println(toIncrementDecrypted + 1);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
        toIncrement++;

        // if (camera.client.available()) {
        //     camera.client.poll();
        // }

        // if (xSemaphoreTake(mutex, 0) == pdTRUE) {
        //     if (sendOpen) {
        //         sendImageFlag = false;
        //         camera.client.send(String(toIncrement));
        //         Serial.print("Number sent from camera: ");
        //         Serial.println(toIncrement);

        //         sendOpen = false;
        //     }
        //     xSemaphoreGive(mutex);
        // }

        // if (sendImageFlag)
        //     camera.sendImageToIndoor();
    }
}

String localMessage;
void onMessageCallback(websockets::WebsocketsMessage message) {
    sendImageFlag = true;
    if (message.isText()) {
        localMessage = message.c_str();
        if (localMessage.toInt() == toIncrement + 1) {
            Serial.print("Received number: ");
            Serial.println(localMessage.toInt());
            toIncrement = toIncrement + 2;
            Serial.print("Internal counter incremented to: ");
            Serial.println(toIncrement);

        } else {
            Serial.println("Unexpected response");
        }
    }
}