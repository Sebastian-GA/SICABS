#include <Arduino.h>

#include "./Tasks/Tasks.hpp"

TaskHandle_t led;
TaskHandle_t video;

// Shared variables for opening door...
bool openDoor = false;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void setup() {
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println();
    Serial.println("---SICABS Indoor---");
    Serial.println();
    // mutex = xSemaphoreCreateMutex();

    if (xTaskCreatePinnedToCore(videoReception, "Video projection", 10000, NULL, 1, &video, 1) != pdPASS) {
        Serial.println("video not allocated :(");
    }

    if (xTaskCreatePinnedToCore(lockControl, "blink LED", 5000, NULL, 1, &led, 0) != pdPASS) {
        Serial.println("LED blinking not allocated :(");
    }

    vTaskDelete(NULL);
}

void loop() {
}

// #include <Arduino.h>

// #include "AES128.h"

// char cleartext[256];  // Buffer to save strings
// char ciphertext[512];
// unsigned long tick, tock;

// void setup() {
//     Serial.begin(115200);
//     while (!Serial)
//         ;
//     delay(2000);

//     aes_init();
//     Serial.println("AES128 Encrypter");
//     Serial.println();
// }

// void loop() {
//     if (Serial.available()) {
//         Serial.println();

//         String readBuffer = Serial.readStringUntil('\n');
//         Serial.println("INPUT: " + readBuffer);

//         // Encrypt
//         sprintf(cleartext, "%s", readBuffer.c_str());  // must not exceed 255 bytes; may contain a newline
//         tick = micros();
//         String encrypted = aes_encrypt(cleartext);
//         tock = micros();
//         Serial.println("Ciphertext: " + encrypted);
//         for (int i = 0; i < encrypted.length(); i++) {
//             Serial.print(encrypted[i], HEX);
//             Serial.print(' ');  // Add a space for better readability
//         }
//         Serial.println("Time taked: " + String(tock - tick) + "uS");

//         // Decrypt
//         sprintf(ciphertext, "%s", encrypted.c_str());
//         tick = micros();
//         String decrypted = aes_decrypt(ciphertext);
//         tock = micros();
//         Serial.println("Cleartext: " + decrypted);
//         Serial.println("Time taked: " + String(tock - tick) + "uS");

//         // Validate
//         if (decrypted.equals(cleartext))
//             Serial.println("SUCCES");
//         else
//             Serial.println("FAILURE");
//     }
// }