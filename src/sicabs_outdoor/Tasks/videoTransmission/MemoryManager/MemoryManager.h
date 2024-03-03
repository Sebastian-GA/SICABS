#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <Preferences.h>

class MemoryManager {
   public:
    MemoryManager();
    String readString();

   private:
    Preferences preferences;
    String message = "hola";
};

#endif