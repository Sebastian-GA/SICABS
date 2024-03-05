#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>

#include <cstring>

#include "./../tasks.hpp"
#include "./gfx/LGFX_ESP32S3_RGB_MakerfabsParallelTFTwithTouch70.h"
#include "common.h"
#include "config.h"
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
String localMessage;
int localInt;

bool sendNow = false;
bool correctMessage = false;
bool incorrectMessage = false;

int toIncrement = 1;
/**************************************************************************
 * CALLBACK FUNCTIONS
 **************************************************************************/
void onMessageCallback(WebsocketsMessage message) {
    if (message.isText()) {
        localMessage = message.c_str();
        if (localMessage.toInt() + 1 == toIncrement) {
            // Toggle sendNow flag so that we can send the response
            Serial.print("Received number: ");
            Serial.println(localMessage.toInt());
            correctMessage = true;
        } else {
            // Unexpected message
            Serial.println("Unexpected message");
            incorrectMessage = true;
        }
    }
    TJpgDec.drawJpg(0, 0, (const uint8_t*)message.c_str(), message.length());
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

    Serial.print("Video task unning on core ");
    Serial.println(xPortGetCoreID());
    Serial.println("Looking for a client...");
    while (!screenClient.available())
        screenClient = server.accept();

    Serial.println("We finally found the client ^^");
    screenClient.onMessage(onMessageCallback);

    for (;;) {
        // screenClient.send("Saludos desde la pantalla");
        // Serial.println("I just sent a message from the big screen");
        if (screenClient.available())
            screenClient.poll();

        if (sendNow) {
            Serial.println("Sending response...");
            screenClient.send("my response");
            sendNow = false;
        }
        if (correctMessage) {
            // send the number
            screenClient.send(String(toIncrement));
            Serial.print("Number sent: ");
            Serial.println(toIncrement);
            // increment the number by 2
            toIncrement = toIncrement + 2;
            Serial.print("Number incremented to: ");
            Serial.println(toIncrement);

            correctMessage = false;
        }
        if (incorrectMessage) {
            // Send a -1
            screenClient.send(String(-1));

            incorrectMessage = false;
        }
    }
}