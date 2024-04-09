#include <Arduino.h>

#include "./../Tasks.hpp"
#include "./Display/Display.h"
#include "./Keyboard/Keyboard.h"
#include "sfm.hpp"

#define SERIAL_BAUD_RATE 115200
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 2

// fingerprint sensor pins
#define SFM_RX 12
#define SFM_TX 16
#define SFM_IRQ -1
#define SFM_VCC 13

Display display;
Keyboard keyboard;
SFM_Module fingerprintSensor = SFM_Module(SFM_VCC, SFM_IRQ, SFM_TX, SFM_RX);

extern bool sendOpen;
extern bool sendFakeOpen;
extern SemaphoreHandle_t mutex;

// for idle state
extern bool idle;
extern SemaphoreHandle_t idleStateMutex;
bool idleInteraction = false;

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
        // check for idle state
        if (xSemaphoreTake(idleStateMutex, 0) == pdTRUE) {
            idleInteraction = idle;
            xSemaphoreGive(idleStateMutex);
        }
        if (idleInteraction) {
            display.drawIdle();
            display.shown = false;
            // keyboard.clearPassword();
            // display.state = State::MENU;
        } else {
            char keyEntered = keyboard.getKey();
            if (keyEntered)
                Serial.println(keyEntered);
            switch (display.state) {
                case State::MENU:
                    display.drawMenu(keyEntered, fingerprintSensor);
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
                    display.touchFingerprint(keyEntered, keyboard, fingerprintSensor);
                    break;
                case State::CORRECT_FINGERPRINT:
                    display.showAccessGranted(keyboard);
                    break;
                case State::INCORRECT_FINGERPRINT:
                    display.showAccessDenied(keyboard);
                    break;
                case State::FAILED_ATTEMPTS:
                    display.failedAttemptsCountdown(keyboard);
                    break;
                default:
                    break;
            }
            // In case the 3 is pressed in the menu, we send the internal message
            if (keyEntered == '4' && display.state == State::MENU) {
                // Take the mutex and toggle the sendOpenFake
                if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
                    // Toggle it in case it's false
                    if (!sendFakeOpen) {
                        sendFakeOpen = true;
                    }
                    xSemaphoreGive(mutex);
                }
            }
        }

        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            // Tell transmission task that it's time to send the message
            if (display.sendSignal) {
                display.sendSignal = false;
                sendOpen = true;
            }
            xSemaphoreGive(mutex);
        }
    }
}
