#include <Arduino.h>

#include "./../tasks.hpp"

// semaphores...
extern bool openDoor;
extern SemaphoreHandle_t mutex;

void lockControl(void *parameter) {
    int time = 5000;
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
    Serial.print("Blinking LED task running on core ");
    Serial.println(xPortGetCoreID());
    for (;;) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            if (openDoor) {
                digitalWrite(led_pin, HIGH);
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                digitalWrite(led_pin, LOW);
                openDoor = false;
            } else {
            }
            xSemaphoreGive(mutex);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        } else {
            // Do something here
        }
    }
}
