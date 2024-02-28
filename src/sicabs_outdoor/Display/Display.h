#ifndef DISPLAY_H
#define DISPLAY_H
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./../Keyboard/Keyboard.h"
#include "DisplayConfig.h"

class Display : public Adafruit_SSD1306 {
   public:
    Display();
    bool shown = false;
    int state = State::MENU;
    unsigned long int startingTime;
    int delayShow = 2000;
    void drawMenu(char keyEntered);
    void drawPin(int digits, int attempts);
    void showGrantedAccess();
    void enterPin(char keyEntered, Keyboard& keyboard);

   private:
    TwoWire* jwire = &Wire;
};

#endif