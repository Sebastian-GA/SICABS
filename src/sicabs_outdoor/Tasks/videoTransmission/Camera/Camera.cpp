#include "Camera.h"

Camera::Camera() {
}

void Camera::initCamera() {
    cameraConfiguration.ledc_channel = LEDC_CHANNEL_0;
    cameraConfiguration.ledc_timer = LEDC_TIMER_0;
    cameraConfiguration.pin_d0 = Y2_GPIO_NUM;
    cameraConfiguration.pin_d1 = Y3_GPIO_NUM;
    cameraConfiguration.pin_d2 = Y4_GPIO_NUM;
    cameraConfiguration.pin_d3 = Y5_GPIO_NUM;
    cameraConfiguration.pin_d4 = Y6_GPIO_NUM;
    cameraConfiguration.pin_d5 = Y7_GPIO_NUM;
    cameraConfiguration.pin_d6 = Y8_GPIO_NUM;
    cameraConfiguration.pin_d7 = Y9_GPIO_NUM;
    cameraConfiguration.pin_xclk = XCLK_GPIO_NUM;
    cameraConfiguration.pin_pclk = PCLK_GPIO_NUM;
    cameraConfiguration.pin_vsync = VSYNC_GPIO_NUM;
    cameraConfiguration.pin_href = HREF_GPIO_NUM;
    cameraConfiguration.pin_sccb_sda = SIOD_GPIO_NUM;
    cameraConfiguration.pin_sccb_scl = SIOC_GPIO_NUM;
    cameraConfiguration.pin_pwdn = PWDN_GPIO_NUM;
    cameraConfiguration.pin_reset = RESET_GPIO_NUM;
    cameraConfiguration.xclk_freq_hz = 10000000;
    cameraConfiguration.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) {
        cameraConfiguration.frame_size = FRAMESIZE_VGA;
        cameraConfiguration.jpeg_quality = 10;  // 0-63 lower number means higher quality
        cameraConfiguration.fb_count = 2;
        cameraConfiguration.grab_mode = CAMERA_GRAB_LATEST;
        cameraConfiguration.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        cameraConfiguration.frame_size = FRAMESIZE_VGA;
        cameraConfiguration.jpeg_quality = 15;
        cameraConfiguration.fb_count = 1;
        cameraConfiguration.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
        cameraConfiguration.fb_location = CAMERA_FB_IN_DRAM;
    }

    esp_err_t err = esp_camera_init(&cameraConfiguration);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t* cameraSensor = esp_camera_sensor_get();
    cameraSensor->set_brightness(cameraSensor, 0);                  // -2 to 2
    cameraSensor->set_contrast(cameraSensor, 0);                    // -2 to 2
    cameraSensor->set_saturation(cameraSensor, 0);                  // -2 to 2
    cameraSensor->set_special_effect(cameraSensor, 0);              // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    cameraSensor->set_whitebal(cameraSensor, 1);                    // 0 = disable , 1 = enable
    cameraSensor->set_awb_gain(cameraSensor, 1);                    // 0 = disable , 1 = enable
    cameraSensor->set_wb_mode(cameraSensor, 0);                     // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    cameraSensor->set_exposure_ctrl(cameraSensor, 1);               // 0 = disable , 1 = enable
    cameraSensor->set_aec2(cameraSensor, 0);                        // 0 = disable , 1 = enable
    cameraSensor->set_ae_level(cameraSensor, 0);                    // -2 to 2
    cameraSensor->set_aec_value(cameraSensor, 300);                 // 0 to 1200
    cameraSensor->set_gain_ctrl(cameraSensor, 1);                   // 0 = disable , 1 = enable
    cameraSensor->set_agc_gain(cameraSensor, 0);                    // 0 to 30
    cameraSensor->set_gainceiling(cameraSensor, (gainceiling_t)0);  // 0 to 6
    cameraSensor->set_bpc(cameraSensor, 0);                         // 0 = disable , 1 = enable
    cameraSensor->set_wpc(cameraSensor, 1);                         // 0 = disable , 1 = enable
    cameraSensor->set_raw_gma(cameraSensor, 1);                     // 0 = disable , 1 = enable
    cameraSensor->set_lenc(cameraSensor, 1);                        // 0 = disable , 1 = enable
    cameraSensor->set_hmirror(cameraSensor, 0);                     // 0 = disable , 1 = enable
    cameraSensor->set_vflip(cameraSensor, 1);                       // 0 = disable , 1 = enable
    cameraSensor->set_dcw(cameraSensor, 1);                         // 0 = disable , 1 = enable
    cameraSensor->set_colorbar(cameraSensor, 0);                    // 0 = disable , 1 = enable
}

void Camera::connectToWifi() {
    WiFi.disconnect(true);
    WiFi.onEvent(wifiEvent);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Camera::wifiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("WiFi connected! IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            break;
    }
}

void Camera::connectClient() {
    while (!client.connect(SERVER_HOST, SERVER_PORT, "/")) {
        Serial.println("Unable to connect. Retrying...");
        delay(100);
    }
}

void Camera::sendImageToIndoor() {
    if (client.available()) {
        camera_fb_t* fb = NULL;
        fb = esp_camera_fb_get();
        client.sendBinary((const char*)fb->buf, fb->len);
        esp_camera_fb_return(fb);
        client.poll();
    }
}

void Camera::sendTextToIndoor() {
    if (client.available()) {
        client.send("Hola");
    }
}