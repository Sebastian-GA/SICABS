/**
 * @file config.cpp
 * @brief Configuration file
 */

#include "config.h"

char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[KEYPAD_ROWS] = {1, 7, 6, 4};  // ROW1 -> P1, ROW2 -> P7, ROW3 -> P6, ROW4 -> P4
byte colPins[KEYPAD_COLS] = {2, 0, 5};     // COL1 -> P2, COL2 -> P0, COL3 -> P5