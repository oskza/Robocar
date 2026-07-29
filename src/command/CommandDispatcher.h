#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H
#include "CommandEnvelope.h"
#include "CommandResponse.h"
#include "robot/RobotCommandHandler.h"
#include "system/SystemCommandHandler.h"
#include "network/WifiCommandHandler.h"
#include "motion/MotionCommandHandler.h"

class CommandDispatcher {
private:
    RobotCommandHandler &_robot;
    MotionCommandHandler &_motion;
    WifiCommandHandler &_wifi;
    SystemCommandHandler &_system;
public:
    CommandDispatcher(
        RobotCommandHandler &robot,
        MotionCommandHandler &motion,
        WifiCommandHandler &wifi,
        SystemCommandHandler &system
    );
    bool dispatch(const CommandEnvelope &command, CommandResponse &response);
};
#endif
