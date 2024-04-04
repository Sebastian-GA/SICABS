#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>

#include <cstring>

#include "./../tasks.hpp"
#include "./gfx/LGFX_ESP32S3_RGB_MakerfabsParallelTFTwithTouch70.h"
#include "EncryptionManager.h"
#include "MemoryManager.h"
#include "config.h"
#include "credentials.h"
#include "credentials_template.h"
/**************************************************************************
 * DEFINITIONS
 **************************************************************************/
LGFX gfx;

const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;

using namespace websockets;
WebsocketsServer server;
WebsocketsClient screenClient;

void onMessageCallback(WebsocketsMessage message);
void onEventsCallback(WebsocketsEvent event, String data);
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

// ============================ SEMAPHORE =============================
extern bool openDoor;
extern SemaphoreHandle_t mutex;

bool correctMessage = false;
bool incorrectMessage = false;

EncryptionManager communicationEncryption;
MemoryManager memoryManager;

// int toIncrement = 1;
unsigned long int lastTimeMessageReceived = 0;
unsigned long int timeOut = 8000;
/**************************************************************************
 * CALLBACK FUNCTIONS
 **************************************************************************/
void onMessageCallback(WebsocketsMessage message) {
    if (message.isText()) {
        String receivedMessage = message.c_str();
        Serial.print("\nMensaje recibido: ");
        Serial.println(receivedMessage);
        // de-encrypt the inner counter
        int internalCounter = memoryManager.readDeencrypted("counter");

        // Compare
        bool correct = receivedMessage == communicationEncryption.encrypt(internalCounter - 1);

        if (correct) {
            Serial.println("\nEs el mensaje de apertura esperado!");
            // Take semaphore and change it
            if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
                openDoor = true;
                xSemaphoreGive(mutex);
            }
            correctMessage = true;
        } else {
            Serial.println("No es el mensaje de apertura esperado...");
            incorrectMessage = true;
        }
    }
    TJpgDec.drawJpg(0, 0, (const uint8_t*)message.c_str(), message.length());

    // record the last time a message was received
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if (event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if (event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    if (y >= gfx.height()) return 0;
    gfx.pushImage(x, y, w, h, bitmap);
    return 1;
}

/**************************************************************************
 * Task function
 **************************************************************************/
void videoReception(void* parameter) {
    communicationEncryption.encryptionManagerInit(communicationAESKey, communicationIV);
    memoryManager.init(screenStorageAESKey, screenStorageIV);
    String readValue = memoryManager.read("counter");

    if (readValue == "notFound") {
        Serial.println("Initializing internal counter to one...");
        memoryManager.writeEncrypted("counter", 1);
    } else {
        Serial.print("The counter has the value of: ");
        Serial.println(memoryManager.readDeencrypted("counter"));
        Serial.print("And encrypted it looks like: ");
        Serial.println(memoryManager.read("counter"));
    }
    gfx.init();
    gfx.setRotation(0);
    gfx.setColorDepth(16);
    gfx.setBrightness(127);
    gfx.fillScreen(TFT_BLACK);
    gfx.setSwapBytes(true);

    gfx.setFont(&fonts::lgfxJapanGothic_16);
    gfx.drawString("SICABS Indoor", 0, 0);

    // JPEG decoder
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tft_output);

    // 1) Sets access point
    WiFi.softAP(wifi_ssid, wifi_password);

    // 2) starts to listen to messages
    server.listen(SERVER_PORT);

    Serial.print("Video task running on core ");
    Serial.println(xPortGetCoreID());
    Serial.println("Looking for a client...");
    while (!screenClient.available())
        screenClient = server.accept();

    Serial.println("We finally found the client ^^");
    screenClient.onMessage(onMessageCallback);

    for (;;) {
        // Serial.println("printed from loop");
        // screenClient.send("Saludos desde la pantalla");
        // Serial.println("I just sent a message from the big screen");
        if (screenClient.available())
            screenClient.poll();
        if (correctMessage) {
            // Get the counter
            int internalCounter = memoryManager.readDeencrypted("counter");
            String internalCounterEncrypted = communicationEncryption.encrypt(internalCounter);
            // Finally send it
            screenClient.send(internalCounterEncrypted);
            Serial.print("Se envió de vuelta el siguiente mensaje: ");
            Serial.println(internalCounterEncrypted);
            Serial.print("Que equivale al número: ");
            Serial.println(internalCounter);

            Serial.print("Contador interno estaba en: ");
            Serial.println(internalCounter);

            // increment the local counter by 2
            internalCounter = internalCounter + 2;
            Serial.print("Ahora se ha incrementado a: ");
            Serial.println(internalCounter);
            Serial.println(" ");

            // Save it internally
            memoryManager.writeEncrypted("counter", internalCounter);

            // Ensures that code executes only once
            correctMessage = false;
        }
        if (incorrectMessage) {
            // Send a -1
            Serial.println("Mensaje de apertura incorrecto.");
            screenClient.send(String(-1));
            Serial.println(" ");

            incorrectMessage = false;
        }
        // Restart if no message has been sent
    }
}