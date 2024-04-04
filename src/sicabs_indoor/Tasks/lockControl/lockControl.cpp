#include <Arduino.h>

#include "./../tasks.hpp"

// semaphores...
extern bool openDoor;
extern SemaphoreHandle_t mutex;

// For button
unsigned long int previousTime = millis();
unsigned long int period = 400;
int buttonPressed = 0;  // Initial state as if not pressed

void lockControl(void *parameter) {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLDOWN);
    digitalWrite(ledPin, LOW);
    for (;;) {
        buttonPressed = digitalRead(buttonPin);
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            if (openDoor || buttonPressed) {
                digitalWrite(ledPin, HIGH);
                vTaskDelay(timeOpen / portTICK_PERIOD_MS);
                digitalWrite(ledPin, LOW);
                openDoor = false;
            }
            xSemaphoreGive(mutex);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}
