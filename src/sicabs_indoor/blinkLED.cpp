#include <Arduino.h>

#include "tasks.hpp"

void blinkLED(void *parameter) {
    int time = 1000;
    pinMode(led_pin, OUTPUT);
    Serial.print("Blinking LED task running on core ");
    Serial.println(xPortGetCoreID());
    for (;;) {
        digitalWrite(led_pin, HIGH);
        vTaskDelay(time / portTICK_PERIOD_MS);
        digitalWrite(led_pin, LOW);
        vTaskDelay(time / portTICK_PERIOD_MS);
    }
}