#ifndef COMMAND_POST_ACTION_H
#define COMMAND_POST_ACTION_H
#include <stdint.h>

enum class CommandPostAction : uint8_t {
    NONE,
    RESTART,
    FACTORY_RESET
};
#endif
