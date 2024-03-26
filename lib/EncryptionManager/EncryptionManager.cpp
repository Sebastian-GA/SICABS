#include "./EncryptionManager.h"

EncryptionManager::EncryptionManager() {
}

String EncryptionManager::encrypt(char *msg) {
    memcpy(aes_key, default_aes_key, 16);
    memcpy(aes_iv, default_aes_iv, 16);
    unsigned int msgLen = strlen(msg);
    unsigned int cipherlength = AESLib::get_cipher64_length(msgLen);
    char encrypted[cipherlength] = {0};  // AHA! needs to be large, 2x is not enough
    // char encrypted[2*msgLen] = {0};

    AESLib::encrypt64((const byte *)msg, msgLen, encrypted, aes_key, sizeof(aes_key), aes_iv);
    return String(encrypted);
}

String EncryptionManager::encrypt(int number) {
    char buffer[10];
    char *msg = dtostrf(number, 0, 0, buffer);
    memcpy(aes_key, default_aes_key, 16);
    memcpy(aes_iv, default_aes_iv, 16);
    unsigned int msgLen = strlen(msg);
    unsigned int cipherlength = AESLib::get_cipher64_length(msgLen);
    char encrypted[cipherlength] = {0};  // AHA! needs to be large, 2x is not enough
    // char encrypted[2*msgLen] = {0};

    AESLib::encrypt64((const byte *)msg, msgLen, encrypted, aes_key, sizeof(aes_key), aes_iv);
    return String(encrypted);
}

String EncryptionManager::decrypt(char *msg) {
    memcpy(aes_key, default_aes_key, 16);
    memcpy(aes_iv, default_aes_iv, 16);
    unsigned int msgLen = strlen(msg);
    char decrypted[msgLen] = {0};

    AESLib::decrypt64(msg, msgLen, (byte *)decrypted, aes_key, sizeof(aes_key), aes_iv);

    unsigned int cipherlength = 0;
    byte padding_length = 0x00;
    for (int i = msgLen - 1; i >= 0; i--) {
        if (decrypted[i] != 0x00) {
            cipherlength = i + 1;
            padding_length = decrypted[i];
            break;
        }
    }
    bool is_valid_padding = true;
    for (int i = cipherlength - padding_length; i < cipherlength - 1; i++) {
        if (decrypted[i] != padding_length) {
            is_valid_padding = false;
            break;
        }
    }
    if (is_valid_padding) {
        decrypted[cipherlength - padding_length] = 0x00;
    }

    return String(decrypted);
}

int EncryptionManager::decrypt(String receivedMessage) {
    char msg[receivedMessage.length() + 1];
    strcpy(msg, receivedMessage.c_str());

    memcpy(aes_key, default_aes_key, 16);
    memcpy(aes_iv, default_aes_iv, 16);
    unsigned int msgLen = strlen(msg);
    char decrypted[msgLen] = {0};

    AESLib::decrypt64(msg, msgLen, (byte *)decrypted, aes_key, sizeof(aes_key), aes_iv);

    unsigned int cipherlength = 0;
    byte padding_length = 0x00;
    for (int i = msgLen - 1; i >= 0; i--) {
        if (decrypted[i] != 0x00) {
            cipherlength = i + 1;
            padding_length = decrypted[i];
            break;
        }
    }
    bool is_valid_padding = true;
    for (int i = cipherlength - padding_length; i < cipherlength - 1; i++) {
        if (decrypted[i] != padding_length) {
            is_valid_padding = false;
            break;
        }
    }
    if (is_valid_padding) {
        decrypted[cipherlength - padding_length] = 0x00;
    }

    String decryptedMessage = String(decrypted);
    return strtol(decryptedMessage.c_str(), NULL, 10);
}

void EncryptionManager::encryptionManagerInit(const byte key[], const byte iv[]) {
    AESLib::set_paddingmode(paddingMode::CMS);
    memcpy(aes_key, key, 16);
    memcpy(default_aes_key, key, 16);
    memcpy(aes_iv, iv, 16);
    memcpy(default_aes_iv, iv, 16);
}