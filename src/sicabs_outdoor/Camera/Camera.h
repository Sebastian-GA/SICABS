#ifndef CAMERA_H
#define CAMERA_H
#include <esp_camera.h>

class Camera {
   public:
    Camera();

   private:
    camera_config_t config;
};

#endif