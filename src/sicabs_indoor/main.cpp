/**
 * @file main.cpp
 * @brief Main file
 */

#include <Arduino.h>
#include <WiFi.h>

#include "common.h"
#include "config.h"
#include "credentials.h"
#include "gfx/LGFX_ESP32S3_RGB_MakerfabsParallelTFTwithTouch70.h"

LGFX gfx;

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Indoor---");
    Serial.println();

    gfx.init();
    gfx.setRotation(0);
    gfx.setColorDepth(16);
    gfx.setBrightness(127);
    gfx.fillScreen(TFT_BLACK);

    gfx.setFont(&fonts::lgfxJapanGothic_16);
    gfx.drawString("SICABS Indoor", 0, 0);
}

void loop() {
}