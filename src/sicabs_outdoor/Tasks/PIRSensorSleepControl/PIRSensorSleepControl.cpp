#include <Arduino.h>

#include "./../Tasks.hpp"

extern bool idle;
extern SemaphoreHandle_t idleStateMutex;

uint8_t PIRPin = 15;
int previousReading = 0;
int currentReading = 0;
int internalCounter = 8;
int prevInternalCounter = 8;
// decrement flag
bool decrementFlag = false;

// variables for the counter
unsigned long int previous = millis();
unsigned long int periodPIR = 1000;

void PIRSensorSleepControl(void* parameter) {
    pinMode(PIRPin, INPUT_PULLDOWN);
    digitalRead(PIRPin);
    while (1) {
        // As loop starts, READ the current state of the PIR sensor
        currentReading = digitalRead(PIRPin);

        // If there is a rising edge
        if (previousReading == 0 && currentReading == 1) {
            Serial.println("Rising edge detected");

            // Do something if the internal counter was in zero
            if (internalCounter <= 0) {
                Serial.println("awake now");
                if (xSemaphoreTake(idleStateMutex, portMAX_DELAY) == pdTRUE) {
                    idle = false;
                    xSemaphoreGive(idleStateMutex);
                }
            }

            // Restart counter
            internalCounter = 8;
            // don't decrement the counter
            decrementFlag = false;
        }

        // if there is a falling edge
        if (previousReading == 1 && currentReading == 0) {
            Serial.println("falling edge detected");
            // Start decrementing
            decrementFlag = true;
        }

        // Decrementing block that will execute depending on the flag
        if (decrementFlag && millis() >= previous + periodPIR && internalCounter > 0) {
            internalCounter = internalCounter - 1;
            if (internalCounter == 0 && prevInternalCounter == 1) {
                Serial.println("Going to sleep...");
                if (xSemaphoreTake(idleStateMutex, portMAX_DELAY) == pdTRUE) {
                    idle = true;
                    xSemaphoreGive(idleStateMutex);
                }
            }

            prevInternalCounter = internalCounter;
            previous = millis();
        }
        // When loop finishes, UPDATE the previous read
        previousReading = currentReading;
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
