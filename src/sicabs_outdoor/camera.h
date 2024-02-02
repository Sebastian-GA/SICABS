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

#include "credentials_template.h"
#include "pin_definitions.h"

using namespace websockets;

void initCamera();
void connectToWifi();
void connectToServer();
void wifiEvent(WiFiEvent_t event);
void onMessageCallback(WebsocketsMessage message);
void onEventsCallback(WebsocketsEvent event, String data);
void sendPicture(void* parameters);

// bool connected;

#endif