#ifndef POWER_COMMAND_HANDLER_H
#define POWER_COMMAND_HANDLER_H
#include "../common/CommandResponse.h"
#include "../CommandResponseBuilder.h"
#include "domain/power/PowerCommand.h"
#include "../../../robot/Robot.h"

class PowerCommandHandler {
public:
    static bool execute(
        Robot &robot,
        PowerCommand command,
        const PowerCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
