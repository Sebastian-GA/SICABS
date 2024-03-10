// #include <Arduino.h>

// #include "./Tasks/Tasks.hpp"
// bool sendOpen = false;
// SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
// bool sendFakeOpen = false;
// void setup() {
//     Serial.begin(115200);
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
//     xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 4000, NULL, 1, NULL, 1);
//     vTaskDelete(NULL);
// }
// void loop() {
// }
#include <Arduino.h>

#include <sfm.hpp>

#define SFM_VCC 13
#define SFM_IRQ -1  // Not connected because of false interrupts
#define SFM_TX 15
#define SFM_RX 12

SFM_Module SFM = SFM_Module(SFM_VCC, SFM_IRQ, SFM_TX, SFM_RX);

uint8_t SFM_return = 0;  // used to get recognition return
uint16_t SFM_uid = 0;    // used to get recognized uid
bool touchState = false;
bool lastTouchState = false;

unsigned long int lastExecutionTime = millis();
unsigned long int period = 1000;

void setup() {
    Serial.begin(115200);
    SFM.setRingColor(SFM_RING_BLUE, SFM_RING_GREEN);
    Serial.print("The UUID is: ");
    Serial.println(SFM.getUuid());
}

void loop() {
    // Execute code every second, not every 1/60 seconds
    if (millis() > lastExecutionTime + period) {
        // If it's pressed, do something
        if (SFM.isFingerPressed()) {
        }
        lastExecutionTime = millis();
    }
}