#include <Arduino.h>

#include "./Tasks/Tasks.hpp"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
bool sendOpen = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
bool sendFakeOpen = false;
void setup() {
    Serial.begin(115200);
    Serial.println("\n\n\n");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(userInteraction, "User interaction", 10000, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(videoTransmission, "Video transmission", 10000, NULL, 1, NULL, 1);
    vTaskDelete(NULL);
}
void loop() {
}

// #include <Arduino.h>

// #include <sfm.hpp>

// // #define SFM_RX 12
// // #define SFM_TX 3
// #define SFM_RX 12
// #define SFM_TX 16
// #define SFM_IRQ -1
// #define SFM_VCC 13

// SFM_Module SFM = SFM_Module(SFM_VCC, SFM_IRQ, SFM_TX, SFM_RX);

// uint8_t temp = 0;      // used to get recognition return
// uint16_t tempUid = 0;  // used to get recognized uid
// bool touchState = false;
// bool lastTouchState = false;

// unsigned long int lastExecutionTime = millis();
// unsigned long int period = 1000;

// enum Phase { zero,
//              first,
//              second,
//              third };

// int phase = Phase::zero;

// void setup() {
//     Serial.begin(115200);
//     // Serial.begin(115200, SERIAL_8N1, 16, 17);
//     // SFM.setRingColor(SFM_RING_BLUE, SFM_RING_OFF);
//     Serial.println("color already set.");
//     Serial.print("The UUID is: ");
//     Serial.println(SFM.getUuid());
//     Serial.print("There are ");
//     Serial.print(SFM.getUserCount());
//     Serial.print(" fingerprints\n");
// }

// void loop() {
//     // Execute inner block every 1 second
//     if (millis() > lastExecutionTime + period) {
//         switch (phase) {
//             case Phase::first:
//                 if (SFM.isFingerPressed()) {
//                     temp = SFM.register_3c3r_1st();
//                     if (temp == SFM_ACK_SUCCESS) {
//                         Serial.println("Successfully recognized. Going to second part...");
//                         phase = Phase::second;
//                     } else {
//                         Serial.println("Not recognized yet");
//                     }
//                 }
//                 break;
//             case Phase::second:
//                 if (SFM.isFingerPressed()) {
//                     temp = SFM.register_3c3r_2nd();
//                     if (temp == SFM_ACK_SUCCESS) {
//                         Serial.println("Successfully recognized. Going to third part...");
//                         phase = Phase::third;
//                     } else {
//                         Serial.println("Recognition failed. Try again.");
//                         phase = Phase::first;
//                     }
//                 }
//                 break;
//             case Phase::third:
//                 if (SFM.isFingerPressed()) {
//                     temp = SFM.register_3c3r_3rd(tempUid);
//                     if (temp == SFM_ACK_SUCCESS && tempUid != 0) {
//                         Serial.printf("Register successful with return UID: %d\n", tempUid);
//                         phase = Phase::first;
//                     } else {
//                         Serial.println("Registration failed. Try again.");
//                         phase = Phase::first;
//                     }
//                 }
//                 break;
//             case Phase::zero:
//                 if (SFM.isTouched() && SFM.isFingerPressed()) {
//                     Serial.println("is touched now.");
//                     SFM.setRingColor(SFM_RING_YELLOW);
//                     temp = SFM.recognition_1vN(tempUid);
//                     if (tempUid != 0) {
//                         Serial.printf("Successfully matched with UID: %d\n", tempUid);
//                         SFM.setRingColor(SFM_RING_GREEN);
//                     } else {
//                         Serial.println("Not recognized xd");
//                         SFM.setRingColor(SFM_RING_RED);
//                     }
//                 }
//                 break;
//         }

//         // Update the time
//         lastExecutionTime = millis();
//     }
// }