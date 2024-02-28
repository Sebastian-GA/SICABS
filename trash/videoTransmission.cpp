#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <esp_camera.h>

#include "credentials.h"
#include "pin_definitions.h"
#include "tasks.hpp"
using namespace websockets;

/**************************************************************************
 * FUNCTION DECLARATIONS
 **************************************************************************/
void initCamera();
void connectToWifi();
void wifiEvent(WiFiEvent_t event);
void onMessageCallback(WebsocketsMessage message);
void onEventsCallback(WebsocketsEvent event, String data);

/**************************************************************************
 * CREDENTIALS
 **************************************************************************/
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
const char* websockets_server_host = SERVER_HOST;
const uint16_t websockets_server_port = SERVER_PORT;
bool connected = false;

WebsocketsClient client;

camera_config_t config;

void videoTransmission(void* parameter) {
    initCamera();
    connectToWifi();
    while (!connected) {
        delay(100);
    }

    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);

    while (!client.connect(websockets_server_host, websockets_server_port, "/")) {
        Serial.println("Not Connected! Retrying connection...");
        delay(100);
    }

    if (client.available()) {
        client.send("Hello Server");
        client.ping();
    }
    for (;;) {
        if (client.available()) {
            camera_fb_t* fb = NULL;
            esp_err_t res = ESP_OK;
            uint32_t t = micros();
            fb = esp_camera_fb_get();
            t = micros() - t;
            Serial.printf("Camera capture time (ms): %d\n", t);

            if (!fb) {
                // Serial.println("Camera capture failed");
                esp_camera_fb_return(fb);
            }

            t = micros();
            client.sendBinary((const char*)fb->buf, fb->len);
            t = micros() - t;
            Serial.printf("Websocket send time (ms): %d\n", t);
            esp_camera_fb_return(fb);

            // client.send("Hello Server");
            // delay(1000);
            client.poll();
        }
    }
}

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        Serial.println(client.getCloseReason());
    } else if (event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if (event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

/**
 * @brief Initialize camera
 *
 */
void initCamera() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 10000000;        // 10000000
    config.pixel_format = PIXFORMAT_JPEG;  // YUV422, GRAYSCALE, RGB565, JPEG

    // init with high specs to pre-allocate larger buffers

    // FRAMESIZE_UXGA (1600 x 1200)
    // FRAMESIZE_QVGA(320 x 240)
    // FRAMESIZE_CIF(352 x 288)
    // FRAMESIZE_VGA(640 x 480)
    // FRAMESIZE_SVGA(800 x 600)
    // FRAMESIZE_XGA(1024 x 768)
    // FRAMESIZE_SXGA(1280 x 1024)

    if (psramFound()) {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 10;  // 0-63 lower number means higher quality
        config.fb_count = 2;
        config.grab_mode = CAMERA_GRAB_LATEST;
        config.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 15;
        config.fb_count = 1;
        config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
        config.fb_location = CAMERA_FB_IN_DRAM;
    }

    // Init Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    // Sensor settings
    sensor_t* s = esp_camera_sensor_get();
    s->set_brightness(s, 0);                  // -2 to 2
    s->set_contrast(s, 0);                    // -2 to 2
    s->set_saturation(s, 0);                  // -2 to 2
    s->set_special_effect(s, 0);              // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);                    // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);                    // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);                     // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);               // 0 = disable , 1 = enable
    s->set_aec2(s, 0);                        // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);                    // -2 to 2
    s->set_aec_value(s, 300);                 // 0 to 1200
    s->set_gain_ctrl(s, 1);                   // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);                    // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);                         // 0 = disable , 1 = enable
    s->set_wpc(s, 1);                         // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);                     // 0 = disable , 1 = enable
    s->set_lenc(s, 1);                        // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);                     // 0 = disable , 1 = enable
    s->set_vflip(s, 1);                       // 0 = disable , 1 = enable
    s->set_dcw(s, 1);                         // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);                    // 0 = disable , 1 = enable
}

/**
 * @brief Connect to WiFi
 *
 */
void connectToWifi() {
    Serial.println("Connecting to WiFi network: " + String(wifi_ssid));

    WiFi.disconnect(true);
    WiFi.onEvent(wifiEvent);
    WiFi.begin(wifi_ssid, wifi_password);

    Serial.println("Waiting for WIFI connection...");
}

/**
 * @brief WiFi event handler
 *
 * @param event
 */
void wifiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("WiFi connected! IP address: ");
            Serial.println(WiFi.localIP());
            connected = true;
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            connected = false;
            break;
    }
}