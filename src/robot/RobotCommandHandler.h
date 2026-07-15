#ifndef ROBOT_COMMAND_HANDLER_H
#define ROBOT_COMMAND_HANDLER_H
#include "command/CommandResponseBuilder.h"
#include "RobotCommand.h"
#include "robot/Robot.h"

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
