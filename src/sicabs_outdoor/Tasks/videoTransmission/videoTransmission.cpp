#include <Arduino.h>
#include <math.h>

#include "./../Tasks.hpp"
#include "./Camera/Camera.h"
#include "EncryptionManager.h"
#include "MemoryManager.h"
#include "credentials.h"

extern bool sendOpen;
extern bool sendFakeOpen;
extern SemaphoreHandle_t mutex;

Camera camera;
EncryptionManager communicationEncryption;
MemoryManager memoryManager;
void onMessageCallback(websockets::WebsocketsMessage message);

bool sendImages = true;

void videoTransmission(void* parameter) {
    Serial.begin(115200);
    // Initialize encrypter
    communicationEncryption.encryptionManagerInit(communicationAESKey, communicationIV);
    memoryManager.init(cameraStorageAESKey, cameraStorageIV);
    // Read the internal counter. If there's nothing, initialize it to 1
    String readValue = memoryManager.read("counter");

    if (readValue == "notFound") {
        Serial.println("Initializing internal counter to ten...");
        memoryManager.writeEncrypted("counter", 60);
    } else {
        Serial.print("\n\nThe counter has the value of: ");
        Serial.println(memoryManager.readDeencrypted("counter"));
        Serial.print("And encrypted it looks like: ");
        Serial.println(memoryManager.read("counter"));
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Initialize camera
    camera.initCamera();
    camera.connectToWifi();
    camera.connectClient();
    camera.client.onMessage(onMessageCallback);

    while (1) {
        if (camera.client.available()) {
            camera.client.poll();
        }

        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            if (sendOpen) {
                // Stop sending images for a while
                sendImages = false;
                // Get internal counter and its encrypted version
                int counter = memoryManager.readDeencrypted("counter");
                String counterToBeTransmitted = communicationEncryption.encrypt(counter);
                // Send it encrypted
                camera.client.send(counterToBeTransmitted);
                // Show what was sent
                Serial.print("\nSent to screen: ");
                Serial.println(counterToBeTransmitted);
                Serial.print("Which is: ");
                Serial.println(counter);

                sendOpen = false;
            }
            if (sendFakeOpen) {
                // Stop sending images for a while
                sendImages = false;
                String internalCounterEncrypted = memoryManager.read("counter");
                Serial.println("Sending counter as encrypted internally...");
                camera.client.send(internalCounterEncrypted);
                sendFakeOpen = false;
            }
            xSemaphoreGive(mutex);
        }

        if (sendImages)
            camera.sendImageToIndoor();
    }
}

void onMessageCallback(websockets::WebsocketsMessage message) {
    // Send images again
    sendImages = true;
    if (message.isText()) {
        String receivedMessage = message.c_str();
        Serial.print("\nMessage received: ");
        Serial.println(receivedMessage);
        int numberDecrypted = communicationEncryption.decrypt(receivedMessage);
        Serial.print("And decrypted looks like: ");
        Serial.println(numberDecrypted);
        int internalCounter = memoryManager.readDeencrypted("counter");

        if (numberDecrypted == internalCounter + 1) {
            Serial.println("Number expected!");
            internalCounter = internalCounter + 2;
            Serial.print("Internal counter incremented to: ");
            Serial.println(internalCounter);
            Serial.println("Saving it to internal memory...");
            memoryManager.writeEncrypted("counter", internalCounter);
            Serial.println(" ");

        } else {
            Serial.println("Unexpected response");
            Serial.println(" ");
        }
    }
}