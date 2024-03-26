/**
 * @file credentials_template.h
 * @brief Credentials file
 *
 * Copy and rename this file to credentials.h and fill in your credentials.
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define SERVER_HOST "192.168.4.1"
#define SERVER_PORT 8888

// ESP32 ID
#define ESP_OUT_CHIP_ID "your_chip_id"
#define ESP_OUT_EFUSE_MAC "your_efuse_mac"
#define ESP_OUT_WIFI_MAC "your_wifi_mac"

#define ESP_IN_CHIP_ID "your_chip_id"
#define ESP_IN_EFUSE_MAC "your_efuse_mac"
#define ESP_IN_WIFI_MAC "your_wifi_mac"

// ESP-NOW
#define ESP_NOW_CHANNEL 1
#define ESP_NOW_PMK "your_pmk"
#define ESP_NOW_LMK "your_lmk"

// Fingerprint Sensor
#define FINGERPRINT_UUID "your_sensor_uuid"

#endif
