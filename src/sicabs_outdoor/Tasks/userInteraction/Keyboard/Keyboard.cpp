#include "Keyboard.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
static char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
static byte rows[KEYPAD_ROWS] = {1, 7, 6, 4};  // ROW0 -> P1, ROW1 -> P7, ROW2 -> P6, ROW3 -> P4
static byte colPins[KEYPAD_COLS] = {2, 0, 5};  // COL0 -> P2, COL1 -> P0, COL2 -> P5
#define KEYPAD_I2C_ADDRESS 0x27

Keyboard::Keyboard() : Keypad_I2C(makeKeymap(KEYPAD_KEYS), rows, colPins, KEYPAD_ROWS, KEYPAD_COLS, KEYPAD_I2C_ADDRESS, PCF8574, &Wire) {
}

void Keyboard::deletePasswordDigit() {
    if (inputPassword.size() > 0)
        inputPassword.pop_back();
}

int Keyboard::getPasswordLength() {
    return inputPassword.size();
}

void Keyboard::clearPassword() {
    inputPassword.clear();
}

void Keyboard::writePasswordDigit(char digit) {
    if (inputPassword.size() < 6)
        inputPassword.push_back(digit);
}

bool Keyboard::checkEnteredPassword() {
    std::string inputPassword2String(inputPassword.begin(), inputPassword.end());
    return inputPassword2String.compare(password) == 0;
}

int Keyboard::getAttempts() {
    return attempts;
}

void Keyboard::removeAttempt() {
    if (attempts > 0) {
        attempts = attempts - 1;
    }
}

void Keyboard::resetAttempts() {
    attempts = 3;
}