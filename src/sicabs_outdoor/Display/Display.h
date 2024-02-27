#ifndef DISPLAY_H
#define DISPLAY_H
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DisplayConfig.h"

class Display : public Adafruit_SSD1306 {
   public:
    Display();
    bool shown = false;
};

#endif