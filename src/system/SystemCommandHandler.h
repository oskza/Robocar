#ifndef SYSTEM_COMMAND_HANDLER_H
#define SYSTEM_COMMAND_HANDLER_H
#include "command/CommandResponseBuilder.h"
#include "SystemCommand.h"
#include "robot/Robot.h"

class SystemCommandHandler {
public:
    static bool execute(
        Robot &robot,
        SystemCommand command,
        const SystemCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
