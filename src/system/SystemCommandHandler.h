#ifndef SYSTEM_COMMAND_HANDLER_H
#define SYSTEM_COMMAND_HANDLER_H
#include "SystemCommand.h"
#include "command/CommandResponse.h"

class SystemCommandHandler {
public:
    bool execute(const SystemCommand &command, CommandResponse &response);
};
#endif
