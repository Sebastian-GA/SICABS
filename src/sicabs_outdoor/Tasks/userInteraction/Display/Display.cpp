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

void Display::showAccessGranted(Keyboard& keyboard) {
    if (!shown) {
        int localVar;
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
        keyboard.resetAttempts();
    }
    if (millis() >= startingTime + delayShow) {
        state = State::MENU;
        shown = false;
    }
}

void Display::showAccessDenied(Keyboard& keyboard) {
    if (!shown) {
        keyboard.removeAttempt();
        if (keyboard.getAttempts() <= 0) {
            state = State::FAILED_ATTEMPTS;
            shown = false;
        }
        shown = true;
        startingTime = millis();

        Adafruit_SSD1306::clearDisplay();
        Adafruit_SSD1306::setTextSize(2);
        Adafruit_SSD1306::setTextColor(WHITE);
        Adafruit_SSD1306::setCursor(25, 10);
        Adafruit_SSD1306::print(F("Access"));
        Adafruit_SSD1306::setCursor(25, 30);
        Adafruit_SSD1306::print(F("denied"));

        Adafruit_SSD1306::display();
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
            state = keyboard.checkEnteredPassword() ? State::CORRECT_PIN : State::INCORRECT_PIN;
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

void Display::drawFailedAttemptsCountdown() {
    Adafruit_SSD1306::clearDisplay();
    Adafruit_SSD1306::setTextSize(2);
    Adafruit_SSD1306::setTextColor(WHITE);
    Adafruit_SSD1306::setCursor(10, 10);
    Adafruit_SSD1306::println(F("Again in:"));
    Adafruit_SSD1306::setCursor(50, 40);
    Adafruit_SSD1306::print(counter);

    Adafruit_SSD1306::display();
}

void Display::failedAttemptsCountdown(Keyboard& keyboard) {
    if (!shown) {
        drawFailedAttemptsCountdown();
        shown = true;
        startingTime = millis();
        counter = prevCounter;
    }

    if (millis() - startingTime >= 1000) {
        startingTime = millis();
        counter = counter - 1;
        drawFailedAttemptsCountdown();
        if (counter <= 0) {
            state = State::MENU;
            counter = 5;
            shown = false;
            keyboard.resetAttempts();
        }
    }
}