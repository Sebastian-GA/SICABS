#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_camera.h>

#include "cameraConfig.h"

class Camera {
   public:
    Camera();
    void initCamera();
    void connectToWifi();
    void wifiEvent(WiFiEvent_t event);

   private:
    camera_config_t cameraConfiguration;
    sensor_t* cameraSensor;
};

#endif