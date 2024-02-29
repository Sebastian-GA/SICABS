#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Keypad_I2C.h>

#include <string>
#include <vector>

#include "KeyboardConfig.h"

class Keyboard : public Keypad_I2C {
   public:
    Keyboard();
    void deletePasswordDigit();
    int getPasswordLength();
    void clearPassword();
    void writePasswordDigit(char digit);
    bool checkEnteredPassword();

    int getAttempts();
    void removeAttempt();
    void resetAttempts();

   private:
    std::vector<char> inputPassword;
    int attempts = 3;
    std::string password = PASSWORD;
};
#endif