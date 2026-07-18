#ifndef COMMAND_DOMAIN_H
#define COMMAND_DOMAIN_H
#include <stdint.h>

enum class CommandDomain : uint8_t {
    UNKNOWN,
    ROBOT,
    SYSTEM,
    MOTION,
    WIFI
};
#endif
