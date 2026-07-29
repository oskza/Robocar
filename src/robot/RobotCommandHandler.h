#ifndef ROBOT_COMMAND_HANDLER_H
#define ROBOT_COMMAND_HANDLER_H
#include "Robot.h"
#include "RobotCommand.h"
#include "RobotStorage.h"
#include "command/CommandResponse.h"

class RobotCommandHandler {
private:
    Robot &_robot;
    RobotStorage &_storage;
public:
    RobotCommandHandler(Robot &robot, RobotStorage &storage);
    bool execute(const RobotCommand &command, CommandResponse &response);
};
#endif
