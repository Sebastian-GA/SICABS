#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <esp_camera.h>

#include "cameraConfig.h"

class Camera {
   public:
    Camera();
    void initCamera();
    void connectToWifi();
    static void wifiEvent(WiFiEvent_t event);
    void connectClient();
    void sendImageToIndoor();

   private:
    camera_config_t cameraConfiguration;
    sensor_t* cameraSensor;
    websockets::WebsocketsClient client;
};

#endif