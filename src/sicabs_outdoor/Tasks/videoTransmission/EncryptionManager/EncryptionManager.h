#ifndef ENCRYPTION_MANAGER_H
#define ENCRYPTION_MANAGER_H

#include <AESLib.h>

class EncryptionManager : public AESLib {
   public:
    EncryptionManager();

   private:
    byte aes_key[];
};

#endif
