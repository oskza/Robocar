#ifndef SYSTEM_COMMAND_HANDLER_H
#define SYSTEM_COMMAND_HANDLER_H
#include "domain/system/SystemCommand.h"
#include "../common/CommandResponse.h"
#include "../../../robot/Robot.h"
#include "../CommandResponseBuilder.h"

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
