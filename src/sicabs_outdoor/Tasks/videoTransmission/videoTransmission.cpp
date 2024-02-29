#include <Arduino.h>

#include "./../Tasks.hpp"

extern int sharedVar;
extern SemaphoreHandle_t mutex;

void videoTransmission(void* parameter) {
    Serial.begin(115200);
    int localVar;
    while (1) {
        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            Serial.print("hi from videoTransmission: ");
            Serial.println(sharedVar);

            // check flag
            // if (display.sendSignal) {
            //     // increment shared variable
            //     localVar = shared_var;
            //     localVar++;
            //     vTaskDelay(500 / portTICK_PERIOD_MS);
            //     shared_var = localVar;

            //     // toggle send flag
            //     display.sendSignal = false;
            // }
            xSemaphoreGive(mutex);
        } else {
        }
    }
}