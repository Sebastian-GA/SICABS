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
    unsigned long int startingTime = 0;
    int delayShow = 2000;
    void drawMenu(char keyEntered);
    void drawPin(int digits, int attempts);
    void showAccessGranted(Keyboard& keyboard);
    void showAccessDenied(Keyboard& keyboard);
    void enterPin(char keyEntered, Keyboard& keyboard);
    void failedAttemptsCountdown(Keyboard& keyboard);
    void drawFailedAttemptsCountdown();

    bool isSent();
    void toggleSent();

   private:
    TwoWire* jwire = &Wire;
    int counter = 5;
    int prevCounter = 5;
    bool sent = false;
};

#endif