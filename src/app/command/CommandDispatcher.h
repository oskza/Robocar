#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H
#include "common/CommandEnvelope.h"
#include "common/CommandResponse.h"
#include "handlers/RobotCommandHandler.h"
#include "handlers/SystemCommandHandler.h"
#include "handlers/MotionCommandHandler.h"
#include "handlers/WifiCommandHandler.h"

class CommandDispatcher {
private:
    RobotCommandHandler &_robot;
    SystemCommandHandler &_system;
    MotionCommandHandler &_motion;
    WifiCommandHandler &_wifi;

public:
    CommandDispatcher(
        RobotCommandHandler &robot,
        SystemCommandHandler &system,
        MotionCommandHandler &motion,
        WifiCommandHandler &wifi
    );

    bool dispatch(const CommandEnvelope &command, CommandResponse &response);
};

#endif
