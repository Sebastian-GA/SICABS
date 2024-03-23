#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
bool sendOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
bool sendFakeOpen = false;

// Timer stuff
uint8_t PIR_PIN = 15;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n\n");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 10000, NULL, 1, NULL, 1);
    vTaskDelete(NULL);
}
void loop() {
}

// #include <Arduino.h>

// uint8_t PIR_PIN = 15;
// // 1) declare the timer
// static TimerHandle_t timerToSleep = NULL;

// // fires up when timer is over
// void goToSleep(TimerHandle_t xTimer);

// // fires up when a motion is detected
// void IRAM_ATTR motionDetected();

// bool hadBeenSlept = false;
// bool executeCode = true;

// void setup() {
//     Serial.begin(115200);
//     // ================== TIMER ============
//     timerToSleep = xTimerCreate(
//         "Sleep timer",
//         10000 / portTICK_PERIOD_MS,
//         pdFALSE,
//         (void *)0,
//         goToSleep);
//     Serial.println("timer has started now");
//     xTimerStart(timerToSleep, portMAX_DELAY);

//     // ================== PIR SENSOR ============
//     pinMode(PIR_PIN, INPUT_PULLDOWN);
//     attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionDetected, CHANGE);
// }

// void loop() {
//     if (executeCode) {
//         Serial.println("Hey mom!!!");
//         delay(10);
//     }
// }

// void goToSleep(TimerHandle_t xTimer) {
//     Serial.println("went to sleep for now...");
//     executeCode = false;
//     // Serial.println("Restarting it...");
//     // xTimerReset(timerToSleep, NULL);
// }

// void IRAM_ATTR motionDetected() {
//     if (digitalRead(PIR_PIN) == HIGH) {
//         xTimerStopFromISR(timerToSleep, NULL);
//         executeCode = true;
//     } else {
//         xTimerResetFromISR(timerToSleep, NULL);
//     }
// }