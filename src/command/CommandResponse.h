#ifndef COMMAND_RESPONSE_H
#define COMMAND_RESPONSE_H
#include <stdint.h>
#include "CommandResponseStatus.h"
#include "CommandResponseType.h"
#include "CommandError.h"
#include "robot/RobotSnapshot.h"
#include "robot/RobotConfig.h"
#include "motion/MotionConfig.h"
#include "network/WifiConfig.h"
#include "network/WifiCredentials.h"

struct CommandResponse {
    uint32_t id;
    CommandResponseStatus status;
    CommandResponseType type;
    CommandError error;
    struct {
        RobotSnapshot robot;
        RobotConfig robotConfig;
        SystemSnapshot system;
        MotionSnapshot motion;
        MotionConfig motionConfig;
        WifiSnapshot wifi;
        WifiConfig wifiConfig;
        WifiCredentials wifiCredentials;
    } payload;
};
#endif
