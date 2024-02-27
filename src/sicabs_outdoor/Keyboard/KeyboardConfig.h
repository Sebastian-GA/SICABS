#ifndef KEYPAD_CONFIG_H
#define KEYPAD_CONFIG_H

#include <Arduino.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[KEYPAD_ROWS] = {1, 7, 6, 4};  // ROW0 -> P1, ROW1 -> P7, ROW2 -> P6, ROW3 -> P4
byte colPins[KEYPAD_COLS] = {2, 0, 5};     // COL0 -> P2, COL1 -> P0, COL2 -> P5
#define KEYPAD_I2C_ADDRESS 0x27

#endif