#ifndef SYSTEM_COMMAND_READ_RESULT_H
#define SYSTEM_COMMAND_READ_RESULT_H
#include "../../domain/system/SystemCommand.h"

struct SystemCommandReadResult {
    bool ok;
    SystemCommand command;
    static SystemCommandReadResult success(const SystemCommand &command) { return { true, command }; }
    static SystemCommandReadResult failure() { return { false, SystemCommand::ping() }; }
};
#endif
