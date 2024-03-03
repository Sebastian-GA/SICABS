#include "MemoryManager.h"

MemoryManager::MemoryManager() {
}

String MemoryManager::readString() {
    String gotString;

    preferences.begin("counter", false);

    if (!preferences.isKey("count")) {
        Serial.println("Key 'count' hadn't been created. Creating it now...");
        preferences.putString("count", message);
    } else {
        Serial.println("No worries, the key was found :)");
    }
    gotString = preferences.getString("count", "Didn't find the value");

    preferences.end();

    return gotString;
}
