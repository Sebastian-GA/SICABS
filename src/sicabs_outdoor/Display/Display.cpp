#include "Display.h"

#include <Wire.h>

Display::Display() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, jwire, OLED_RESET) {
}

void Display::drawMenu(char keyEntered) {
    if (!shown) {
        Adafruit_SSD1306::clearDisplay();
        Adafruit_SSD1306::setTextSize(2);
        Adafruit_SSD1306::setTextColor(WHITE);
        Adafruit_SSD1306::setCursor(10, 10);
        Adafruit_SSD1306::println(F("Press:"));
        Adafruit_SSD1306::println(F("1. PIN"));
        Adafruit_SSD1306::println(F("2. Finger"));
        Adafruit_SSD1306::display();
        shown = true;
    }
    if (keyEntered == '1') {
        state = State::ENTER_PIN;
        shown = false;
    }
    if (keyEntered == '2') {
        state = State::ENTER_FINGERPRINT;
        shown = false;
    }
}

void Display::drawPin(int digits, int attempts) {
    Adafruit_SSD1306::clearDisplay();
    Adafruit_SSD1306::setTextSize(2);
    Adafruit_SSD1306::setTextColor(WHITE);
    Adafruit_SSD1306::setCursor(5, 0);
    Adafruit_SSD1306::println(F("Enter PIN:"));
    Serial.println("are you drawing?");
    Adafruit_SSD1306::setCursor(25, 21);
    for (int i = 0; i < digits; i++) {
        Adafruit_SSD1306::print('*');
    }
    Adafruit_SSD1306::setCursor(30, 42);
    Adafruit_SSD1306::print(F("Att: "));
    Adafruit_SSD1306::print(attempts);

    Adafruit_SSD1306::display();
}

void Display::showGrantedAccess() {
    if (!shown) {
        Adafruit_SSD1306::clearDisplay();
        Adafruit_SSD1306::setTextSize(2);
        Adafruit_SSD1306::setTextColor(WHITE);
        Adafruit_SSD1306::setCursor(25, 10);
        Adafruit_SSD1306::print(F("Access"));
        Adafruit_SSD1306::setCursor(20, 30);
        Adafruit_SSD1306::print(F("granted"));

        Adafruit_SSD1306::display();
        shown = true;
        startingTime = millis();
    }
    if (millis() >= startingTime + delayShow) {
        state = State::MENU;
        shown = false;
    }
}

void Display::enterPin(char keyEntered, Keyboard& keyboard) {
    if (!shown) {
        drawPin(keyboard.getPasswordLength(), keyboard.getAttempts());
        shown = true;
    }
    if (keyEntered) {
        if (keyEntered == '*') {
            keyboard.deletePasswordDigit();
            drawPin(keyboard.getPasswordLength(), keyboard.getAttempts());
        } else if (keyEntered == '#' && keyboard.getPasswordLength() >= 6) {
            Serial.println(keyboard.getPasswordLength());
            state = State::CORRECT_PIN;
            keyboard.clearPassword();
            shown = false;
        } else if (keyEntered == '#') {
            keyboard.clearPassword();
            state = State::MENU;
            shown = false;
        } else {
            keyboard.writePasswordDigit(keyEntered);
            drawPin(keyboard.getPasswordLength(), keyboard.getAttempts());
        }
    }
}