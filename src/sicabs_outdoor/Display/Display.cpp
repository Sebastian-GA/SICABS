#include "Display.h"

#include <Wire.h>

Display::Display() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, jwire, OLED_RESET) {
}