#include <Arduino.h>
#include <ArduinoWebsockets.h>

#include "./../Tasks.hpp"

const char* ssid = "thisisnice";
const char* password = "thisisnice";

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

void dataReception(void* parameter) {
    WiFi.softAP(ssid, password);
    IPAddress ip = WiFi.softAPIP();

    // server.listen(4040);

    while (1) {
        Serial.print("The IP is: ");
        Serial.println(ip);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}