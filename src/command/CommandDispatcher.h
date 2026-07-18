#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H
#include "CommandEnvelope.h"
#include "CommandResponse.h"
#include "motion/MotionController.h"
#include "motion/MotionStorage.h"
#include "network/WifiController.h"
#include "network/WifiStorage.h"
#include "robot/Robot.h"
#include "robot/RobotStorage.h"

class CommandDispatcher {
private:
    Robot &_robot;
    RobotStorage &_robotStorage;
    MotionController &_motion;
    MotionStorage &_motionStorage;
    WifiController &_wifi;
    WifiStorage &_wifiStorage;
public:
    CommandDispatcher(
        Robot &robot,
        RobotStorage &robotStorage,
        MotionController &motion,
        MotionStorage &motionStorage,
        WifiController &wifi,
        WifiStorage &wifiStorage
    );
    bool dispatch(const CommandEnvelope &command, CommandResponse &response);
};
#endif
