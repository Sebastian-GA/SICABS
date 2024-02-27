/**
 * @file config.h
 * @brief Configuration file
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Keypad

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[KEYPAD_ROWS] = {1, 7, 6, 4};  // ROW0 -> P1, ROW1 -> P7, ROW2 -> P6, ROW3 -> P4
byte colPins[KEYPAD_COLS] = {2, 0, 5};     // COL0 -> P2, COL1 -> P0, COL2 -> P5

#define KEYPAD_DEBOUNCE_TIME_MS 10  // Default by library is 10
#define KEYPAD_HOLD_TIME_MS 500     // Default by library is 500

/**************************************************************************
 * CONSTANTS
 **************************************************************************/

// Deep Sleep

#define SLEEP_DELAY_MS 10000  // Time to go to sleep after last motion detection

// Keypad

#define PASSWORD_LENGTH 6

// State Machine

enum FSMState {
    FSM_IDLE,
    FSM_TOCK_TOCK,
    FSM_WAIT_FOR_INPUT,
    FSM_VALIDATE_PASSWORD,

    FSM_REGISTER_NEW_FINGERPRINT,
    FSM_REGISTER_NEW_PASSWORD,
};

#endif