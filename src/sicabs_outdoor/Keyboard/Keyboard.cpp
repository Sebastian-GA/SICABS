#include "Keyboard.h"

Keyboard::Keyboard() : Keypad_I2C(makeKeymap(KEYPAD_KEYS), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADDRESS, PCF8574, &Wire) {
}