#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SCREEN_I2C_ADDRESS 0x3C
#define FREQUENCY 100000

TwoWire* twowire = &Wire;

#endif
