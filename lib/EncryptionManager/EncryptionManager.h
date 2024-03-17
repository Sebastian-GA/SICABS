#ifndef ENCRYPTION_MANAGER_H
#define ENCRYPTION_MANAGER_H
#include <AESLib.h>

class EncryptionManager : public AESLib {
   public:
    EncryptionManager();
    String encrypt(char *msg);
    String encrypt(int number);

    String decrypt(char *msg);
    int decrypt(String receivedMessage);
    void encryptionManagerInit(const byte key[], const byte iv[]);

   private:
    byte aes_iv[N_BLOCK];
    byte aes_key[N_BLOCK];
    byte default_aes_iv[N_BLOCK];
    byte default_aes_key[N_BLOCK];
};

#endif
