/**
 * @file pin_definitions.h
 * @brief Pin definitions
 */

#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

#include <Arduino.h>

// Camera Model

#define CAMERA_MODEL_AI_THINKER

#ifdef CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define LED_FLASH_GPIO_NUM 4
#define LED_BUILTIN 33
#define BTN_BUILTIN 0
#else
#error "Camera model not selected"
#endif

// I2C

#define I2C_SDA 14
#define I2C_SCL 2

#define KEYPAD_ADDR 0x27

// Fingerprint

#define SFM_VCC 13
#define SFM_IRQ -1  // Not connected because of false interrupts
#define SFM_TX 16
#define SFM_RX 12

// PIR Sensor

#define PIR_PIN 15

// Wakeup Sources
// PIR Sensor is used as wakeup sources

#define WAKEUP_PIN_BITMASK 0x8000  // HEX(2^15)

#endif  // PIN_DEFINITIONS_H