#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H
#include <stdint.h>

enum class SystemCommandType : uint8_t {
    UNKNOWN,
    RESTART,
    FACTORY_RESET
};

struct SystemCommandPayload {};

struct SystemCommand {
    SystemCommandType type = SystemCommandType::UNKNOWN;
    SystemCommandPayload payload{};
};
#endif
