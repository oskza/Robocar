#ifndef COMMAND_RESPONSE_H
#define COMMAND_RESPONSE_H
#include <stdint.h>
#include "CommandResponseStatus.h"
#include "CommandResponseType.h"
#include "CommandError.h"
#include "../../../robot/RobotSnapshot.h"
#include "../../../subsystems/system/SystemSnapshot.h"
#include "../../../subsystems/motion/MotionSnapshot.h"
#include "../../../subsystems/network/WifiSnapshot.h"
#include "../../../subsystems/network/WifiConfig.h"
#include "../../../subsystems/network/WifiCredentials.h"

struct CommandResponse {
    uint32_t id;
    CommandResponseStatus status;
    CommandResponseType type;
    CommandError error;
    union {
        RobotSnapshot robot;
        SystemSnapshot system;
        MotionSnapshot motion;
        WifiSnapshot wifi;
        WifiConfig wifiConfig;
        WifiCredentials wifiCredentials;
    } payload;
};
#endif
