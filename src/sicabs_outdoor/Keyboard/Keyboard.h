#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Keypad_I2C.h>

#include <vector>

class Keyboard : public Keypad_I2C {
   public:
    Keyboard();
    void deletePasswordDigit();
    int getPasswordLength();
    void clearPassword();
    void writePasswordDigit(char digit);
    int getAttempts();

   private:
    std::vector<char> inputPassword;
    int attempts = 3;
};
#endif