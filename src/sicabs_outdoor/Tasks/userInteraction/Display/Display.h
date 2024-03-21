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
    // Draw functions
    void drawMenu(char keyEntered);
    void drawPin(int digits, int attempts);
    void drawFailedAttemptsCountdown();
    void drawTouchPromptText(int attempts);

    // for fingerprint sensor
    void touchFingerprint(char keyEntered, Keyboard& keyboard);

    void enterPin(char keyEntered, Keyboard& keyboard);
    void failedAttemptsCountdown(Keyboard& keyboard);
    void showAccessGranted(Keyboard& keyboard);
    void showAccessDenied(Keyboard& keyboard);

    bool sendSignalFlag();
    void toggleSendSignalFlag();

    bool sendSignal = false;

   private:
    TwoWire* jwire = &Wire;
    int counter = 30;
    int prevCounter = 30;
};

#endif