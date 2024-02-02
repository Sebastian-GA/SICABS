#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverAddress[] = {0xF4, 0x12, 0xFA, 0xF8, 0xD6, 0x50};

static const char* PMK_KEY = "123hjk9i0ii67bte";
static const char* LMK_KEY = "0ii67bte123hjk9i";

typedef struct struct_message {
    int count;
    int num1;
    int num2;
} struct_message;

struct_message message;
int count;

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

esp_now_peer_info_t peerInfo;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("There was an error initializing ESP-NOW");
        return;
    }

    esp_now_set_pmk((uint8_t*)PMK_KEY);

    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;

    for (uint8_t i = 0; i < 16; i++) {
        peerInfo.lmk[i] = LMK_KEY[i];
    }

    peerInfo.encrypt = true;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    static unsigned long lastEventTime = millis();
    if ((millis() - lastEventTime) > 5000) {
        lastEventTime = millis();
        message.count = count++;
        message.num1 = random(0, 100);
        message.num2 = random(0, 100);

        esp_err_t result = esp_now_send(receiverAddress, (uint8_t*)&message, sizeof(message));
        if (result == ESP_OK) {
            Serial.println("Sent with success");
        } else {
            Serial.println("Error sending the data");
        }
    }
}