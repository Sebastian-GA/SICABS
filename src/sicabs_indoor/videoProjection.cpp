#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>

#include <cstring>

#include "common.h"
#include "config.h"
#include "credentials_template.h"
#include "gfx/LGFX_ESP32S3_RGB_MakerfabsParallelTFTwithTouch70.h"
#include "tasks.hpp"
/**************************************************************************
 * DEFINITIONS
 **************************************************************************/
LGFX gfx;

const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

void onMessageCallback(WebsocketsMessage message);
void onEventsCallback(WebsocketsEvent event, String data);
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

// ============================ SEMAPHORE =============================
extern bool openDoor;
extern SemaphoreHandle_t mutex;
/**************************************************************************
 * CALLBACK FUNCTIONS
 **************************************************************************/
void onMessageCallback(WebsocketsMessage message) {
    if (message.isText()) {
        // while (!connected) {
        //     Serial.println("n-execution");
        // Takes mutex for toggling open flag
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            Serial.println("semaphore has been taken now");
            openDoor = true;
            xSemaphoreGive(mutex);
        } else {
            Serial.println("not taken yet...");
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
void videoProjection(void* parameter) {
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

    // WiFi
    WiFi.softAP(wifi_ssid, wifi_password);
    IPAddress IP = WiFi.softAPIP();

    // WebSockets
    server.listen(SERVER_PORT);

    Serial.print("Video task running on core ");
    Serial.println(xPortGetCoreID());

    // client.onMessage(onMessageCallback);

    for (;;) {
        // client = server.accept();
        // if (client.available()) {
        //     Serial.println("its available");
        //     websockets::WebsocketsMessage msg = client.readBlocking();

        //     Serial.println(msg.data());

        //     // client.close();
        // } else {
        //     Serial.println("not found...");
        // }
        // vTaskDelay(500 / portTICK_PERIOD_MS);

        // Serial.println("Executed loop...");
        // if (server.poll()) {
        //     Serial.println("true");
        //     client = server.accept();
        //     client.onMessage(onMessageCallback);
        // } else {
        //     Serial.println("false");
        // }

        // if (client.available()) {
        //     Serial.println("before poll...");
        //     client.poll();
        //     Serial.println("after poll...");
        // }
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        if (server.poll()) {
            // if (!client.available()) {  // Always overwrite the client to the new one
            client = server.accept();
            client.onMessage(onMessageCallback);
            client.onEvent(onEventsCallback);
            client.send("Hello Client!");
            // }
        }

        if (client.available()) {
            client.poll();
        }
    }
}