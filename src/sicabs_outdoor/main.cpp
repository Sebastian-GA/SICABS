#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    vTaskDelete(NULL);
}

void loop() {
}