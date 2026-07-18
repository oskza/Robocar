#ifndef SYSTEM_COMMAND_HANDLER_H
#define SYSTEM_COMMAND_HANDLER_H
#include "SystemCommand.h"
#include "command/CommandResponse.h"

namespace SystemCommandHandler {
    bool execute(SystemCommand command, CommandResponse &response);
}
#endif
