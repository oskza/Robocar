#ifndef COMMAND_ERROR_H
#define COMMAND_ERROR_H
#include <stdint.h>

enum class CommandError : uint16_t {
    NONE,
    INVALID_JSON,
    INVALID_ENVELOPE,
    INVALID_COMMAND,
    INVALID_PAYLOAD,
    STORAGE_ERROR,
    APPLY_ERROR,
    SYSTEM_ERROR
};
#endif
