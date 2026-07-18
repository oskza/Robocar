#ifndef COMMAND_RESPONSE_H
#define COMMAND_RESPONSE_H
#include "CommandResponseStatus.h"
#include "CommandResponseType.h"
#include "CommandError.h"
#include "CommandPostAction.h"
#include "robot/RobotSnapshot.h"
#include "robot/RobotConfig.h"
#include "motion/MotionConfig.h"
#include "network/WifiConfig.h"
#include "network/WifiCredentials.h"

struct CommandResponsePayload {
    RobotSnapshot robot{};
    RobotConfig robotConfig{};
    MotionSnapshot motion{};
    MotionConfig motionConfig{};
    WifiConfig wifiConfig{};
    WifiCredentials wifiCredentials{};
};

struct CommandResponse {
    uint32_t id = 0;
    CommandResponseStatus status = CommandResponseStatus::UNKNOWN;
    CommandResponseType type = CommandResponseType::UNKNOWN;
    CommandError error = CommandError::NONE;
    CommandPostAction postAction = CommandPostAction::NONE;
    CommandResponsePayload payload{};
};
#endif
