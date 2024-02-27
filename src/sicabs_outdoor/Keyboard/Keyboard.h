#ifndef KEYPAD_H
#define KEYPAD_H

#include <Keypad.h>
#include <Keypad_I2C.h>

#include "KeyboardConfig.h"

#define makeKeymap(x) ((char*)x)

class Keyboard : public Keypad_I2C {
   public:
    Keyboard();
};

#endif