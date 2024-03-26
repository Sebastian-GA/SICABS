#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <Preferences.h>

#include "EncryptionManager.h"

class MemoryManager {
   public:
    MemoryManager();
    void init(const byte key[], const byte iv[]);
    String read(const char* key);
    int readDeencrypted(const char* key);
    void write(const char* key, String input);
    void writeEncrypted(const char* key, int input);

   private:
    Preferences preferences;
    EncryptionManager encryptionManager;
};

#endif
