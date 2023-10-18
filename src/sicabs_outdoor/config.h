/**
 * @file config.h
 * @brief Configuration file
 */

#ifndef CONFIG_H
#define CONFIG_H

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

// Keypad
#define KEYPAD_ADDR 0x27

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
extern char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS];
extern byte rowPins[KEYPAD_ROWS];
extern byte colPins[KEYPAD_COLS];

#define KEYPAD_DEBOUNCE_TIME_MS 10  // Default by library is 10
#define KEYPAD_HOLD_TIME_MS 500     // Default by library is 500

// Fingerprint
#define SFM_VCC 0  // Don't care about this definition, just connect VCC to 3.3V
#define SFM_IRQ 13
#define SFM_TX 16
#define SFM_RX 12

// WakeUp Sources
#define PIR_PIN 15
#define WAKEUP_PIN_BITMASK 0xA000  // 2^13 + 2^15
#define SLEEP_DELAY_MS 10000       // Time to go to sleep after last motion detection

#endif