#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Keypad_I2C.h>

#include "KeyboardConfig.h"

class Keyboard : public Keypad_I2C {
   public:
    Keyboard();
};
#endif