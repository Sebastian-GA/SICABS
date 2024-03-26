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
            Serial.print("mutex taken from blink LED: ");
            if (openDoor) {
                digitalWrite(led_pin, HIGH);
                Serial.println("Open now. Closing in 5 seconds...");
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                digitalWrite(led_pin, LOW);
                openDoor = false;
                Serial.println("Closed now.");
                //     vTaskDelay(time / portTICK_PERIOD_MS);
                //     openDoor = false;
            } else {
                Serial.println("Not open yet...");
            }
            xSemaphoreGive(mutex);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        } else {
            // Do something here
        }
    }
}
