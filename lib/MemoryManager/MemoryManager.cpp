#include "MemoryManager.h"

MemoryManager::MemoryManager() {
}

void MemoryManager::init(const byte key[], const byte iv[]) {
    encryptionManager.encryptionManagerInit(key, iv);
}
String MemoryManager::read(const char* key) {
    // We're only going to read data
    preferences.begin("esp32cam", true);
    String gotValue = preferences.getString(key, "notFound");
    preferences.end();

    return gotValue;
}
int MemoryManager::readDeencrypted(const char* key) {
    // We're only going to read data
    preferences.begin("esp32cam", true);
    String gotValue = preferences.getString(key, "notFound");
    preferences.end();
    if (gotValue == "notFound")
        return -1;

    return encryptionManager.decrypt(gotValue);
}
void MemoryManager::write(const char* key, String input) {
    preferences.begin("esp32cam", false);
    preferences.putString(key, input);
    preferences.end();
}
void MemoryManager::writeEncrypted(const char* key, int input) {
    // 1) Encrypt the number
    String encryptedNumber = encryptionManager.encrypt(input);
    preferences.begin("esp32cam", false);
    preferences.putString(key, encryptedNumber);
    preferences.end();
}