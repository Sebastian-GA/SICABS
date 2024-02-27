#include <Arduino.h>

#include "./Display/Display.h"
#include "./Keyboard/Keyboard.h"

#define SERIAL_BAUD_RATE 115200
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 2

Display display;
Keyboard keyboard;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, FREQUENCY);

    // Display initialization
    bool displayRAMAllocation = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS);
    if (!displayRAMAllocation) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.display();
    keyboard.begin();
}

void loop() {
    char keyEntered = keyboard.getKey();
    if (keyEntered) Serial.println(keyEntered);
}
