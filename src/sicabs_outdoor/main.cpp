#include <Arduino.h>
#include <Keypad_I2C.h>

#define SICABS_OUTDOOR
#include "config.h"
#include "credentials.h"

#define keypad_addr 0x20  // I2C address of I2C Expander module (A0-A1-A2 dip switch to off position)
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

// Keypad pins connected to the I2C-Expander pins P0-P6
byte rowPins[ROWS] = {0, 1, 2, 3};  // connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6};     // connect to the column pinouts of the keypad

// Create instance of the Keypad name I2C_Keypad and using the PCF8574 chip
Keypad_I2C I2C_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr, PCF8574);

void setup() {
    I2C_Keypad.begin();
}

void loop() {
    char key = I2C_Keypad.getKey();
}
