#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H
#include <stdint.h>

enum class SystemCommandType : uint8_t { PING, RESTART, FACTORY_RESET };

struct SystemCommand {
    const SystemCommandType type;
    explicit SystemCommand(SystemCommandType commandType) : type(commandType) {}
    static SystemCommand ping() { return SystemCommand(SystemCommandType::PING); }
    static SystemCommand restart() { return SystemCommand(SystemCommandType::RESTART); }
    static SystemCommand factoryReset() { return SystemCommand(SystemCommandType::FACTORY_RESET); }
};
#endif
