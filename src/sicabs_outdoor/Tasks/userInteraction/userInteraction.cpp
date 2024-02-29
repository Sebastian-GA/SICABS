#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Display/Display.h"
#include "./Keyboard/Keyboard.h"

#define SERIAL_BAUD_RATE 115200
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 2

Display display;
Keyboard keyboard;

extern int sharedVar;
extern SemaphoreHandle_t mutex;

void userInteraction(void* parameter) {
    Serial.begin(SERIAL_BAUD_RATE);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, FREQUENCY);

    bool displayRAMAllocation = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS);
    if (!displayRAMAllocation) {
        Serial.println(F("Display allocation failed"));
        for (;;)
            ;
    }
    keyboard.begin();
    delay(50);
    Serial.print(display.state);
    int localVar;

    while (1) {
        char keyEntered = keyboard.getKey();
        switch (display.state) {
            case State::MENU:
                display.drawMenu(keyEntered);
                break;
            case State::ENTER_PIN:
                display.enterPin(keyEntered, keyboard);
                break;
            case State::CORRECT_PIN:
                display.showAccessGranted(keyboard);
                break;
            case State::INCORRECT_PIN:
                display.showAccessDenied(keyboard);
                break;
            case State::ENTER_FINGERPRINT:
                break;
            case State::CORRECT_FINGERPRINT:
                break;
            case State::INCORRECT_FINGERPRINT:
                break;
            case State::FAILED_ATTEMPTS:
                display.failedAttemptsCountdown(keyboard);
                break;
            default:
                break;
        }
        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            if (!display.sendSignal) {
            } else {
                display.sendSignal = false;
                sharedVar++;
            }
            xSemaphoreGive(mutex);
        } else {
        }
    }
}