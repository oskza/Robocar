#ifndef COMMAND_ERROR_H
#define COMMAND_ERROR_H
#include <stdint.h>

enum class CommandError : uint16_t {
    NONE,
    INVALID_COMMAND,
    INVALID_PAYLOAD,
    STORAGE_ERROR,
    SYSTEM_ERROR,
    WIFI_INVALID_CREDENTIALS,
    WIFI_NOT_CONNECTED
};
#endif
