#include <WiFi.h>
#include <esp_now.h>

uint8_t masterMacAddress[] = {0xE0, 0x5A, 0x1B, 0xD2, 0xB3, 0xCC};

static const char* PMK_KEY = "123hjk9i0ii67bte";
static const char* LMK_KEY = "0ii67bte123hjk9i";

typedef struct struct_message {
    int count;
    int num1;
    int num2;
} struct_message;

struct_message message;

void printMAC(const uint8_t* mac_addr) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
}

void OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) {
    Serial.print("Packet received from: ");
    printMAC(mac_addr);

    memcpy(&message, incomingData, sizeof(message));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Packet number: ");
    Serial.println(message.count);
    Serial.print("Number 1: ");
    Serial.println(message.num1);
    Serial.print("Number 2: ");
    Serial.println(message.num2);
    Serial.println();
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

    memcpy(peerInfo.peer_addr, masterMacAddress, 6);
    peerInfo.channel = 0;

    for (uint8_t i = 0; i < 16; i++) {
        peerInfo.lmk[i] = LMK_KEY[i];
    }

    peerInfo.encrypt = true;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}