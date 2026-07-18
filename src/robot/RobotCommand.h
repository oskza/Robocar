#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H
#include "RobotConfig.h"

enum class RobotCommandType : uint8_t {
    UNKNOWN,
    STATUS,
    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,
    ENABLE_TELEMETRY,
    DISABLE_TELEMETRY
};

struct RobotCommandPayload {
    RobotConfig config{};
};

struct RobotCommand {
    RobotCommandType type = RobotCommandType::UNKNOWN;
    RobotCommandPayload payload{};
};
#endif
