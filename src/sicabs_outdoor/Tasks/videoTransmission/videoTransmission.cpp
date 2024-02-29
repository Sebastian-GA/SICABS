#include <Arduino.h>

#include "./../Tasks.hpp"

extern int shared_var;
extern SemaphoreHandle_t mutex;

void videoTransmission(void* parameter) {
    int local_var;
    while (1) {
        while (!xSemaphoreTake(mutex, 0) != pdTRUE) {
            ;
        }
        Serial.println(shared_var);
        xSemaphoreGive(mutex);
    }
}