
/**
 * @file credentials_template.h
 * @brief Credentials file
 *
 * Copy and rename this file to credentials.h and fill in your credentials.
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define SERVER_HOST "your_server_host"
#define SERVER_PORT 1111

// Fingerprint Sensor
#define FINGERPRINT_UUID "your_sensor_uuid"

<<<<<<< HEAD
const byte communicationAESKey[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte communicationIV[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte cameraStorageAESKey[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte cameraStorageIV[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte screenStorageAESKey[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte screenStorageIV[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
=======
const byte communicationAESKey[] = {0x33, 0x37, 0x31, 0x35, 0x38, 0x32, 0x30, 0x34, 0x39, 0x36, 0x33, 0x31, 0x37, 0x32, 0x38, 0x35};
const byte communicationAESKey[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

const byte communicationIV[] = {0x36, 0x32, 0x38, 0x31, 0x34, 0x39, 0x33, 0x30, 0x37, 0x35, 0x32, 0x38, 0x31, 0x35, 0x34, 0x39};

const byte cameraStorageAESKey[] = {0x34, 0x37, 0x32, 0x31, 0x38, 0x36, 0x33, 0x30, 0x35, 0x39, 0x32, 0x34, 0x31, 0x37, 0x38, 0x33};

const byte cameraStorageIV[] = {0x39, 0x33, 0x30, 0x36, 0x32, 0x38, 0x34, 0x31, 0x37, 0x35, 0x32, 0x39, 0x33, 0x38, 0x31, 0x34};

const byte screenStorageAESKey[] = {0x35, 0x38, 0x33, 0x31, 0x37, 0x34, 0x39, 0x32, 0x30, 0x36, 0x33, 0x38, 0x31, 0x35, 0x37, 0x34};

const byte screenStorageIV[] = {0x36, 0x32, 0x38, 0x33, 0x37, 0x31, 0x35, 0x39, 0x34, 0x30, 0x32, 0x37, 0x31, 0x36, 0x33, 0x38};
>>>>>>> 4e2cedaec7ccf82c6f00bdd228d869b3ddb68746
#endif
