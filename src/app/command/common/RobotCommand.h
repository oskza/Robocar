#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H
#include "../../../robot/RobotConfig.h"

enum class RobotCommand : uint8_t {
    STATUS,
    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,
    ENABLE_TELEMETRY,
    DISABLE_TELEMETRY
};

struct RobotCommandPayload {
    RobotConfig cfg;
};
#endif
