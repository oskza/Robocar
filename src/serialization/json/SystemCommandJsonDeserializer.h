#ifndef SYSTEM_COMMAND_JSON_DESERIALIZER_H
#define SYSTEM_COMMAND_JSON_DESERIALIZER_H
#include <Arduino.h>
#include "SystemCommandReadResult.h"

class SystemCommandJsonDeserializer {
public:
    static SystemCommandReadResult read(const char *data, size_t len);
};
#endif
