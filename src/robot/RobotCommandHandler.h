#ifndef ROBOT_COMMAND_HANDLER_H
#define ROBOT_COMMAND_HANDLER_H
#include "Robot.h"
#include "RobotCommand.h"
#include "RobotStorage.h"
#include "command/CommandResponse.h"

namespace RobotCommandHandler {
    bool execute(Robot &robot, RobotStorage &storage, const RobotCommand &command, CommandResponse &response);
}
#endif
