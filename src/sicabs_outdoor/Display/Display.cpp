#include "Display.h"

#include <Wire.h>

#include "DisplayConfig.h"

Display::Display() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}