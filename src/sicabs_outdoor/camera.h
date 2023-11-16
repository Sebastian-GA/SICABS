/**
 * @file camera.h
 * @brief Camera related functions
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <esp_camera.h>

#include "credentials.h"
#include "pin_definitions.h"

void initCamera();
void connectToWifi();
void wifiEvent(WiFiEvent_t event);
void sendPicture(void* parameters);

#endif