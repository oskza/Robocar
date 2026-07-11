#ifndef ROBOT_COMMAND_HANDLER_H
#define ROBOT_COMMAND_HANDLER_H
#include "../common/RobotCommand.h"
#include "../common/CommandResponse.h"
#include "../../../robot/Robot.h"
#include "../CommandResponseBuilder.h"

class RobotCommandHandler {
public:
    static bool execute(
        Robot &robot,
        RobotCommand command,
        const RobotCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
