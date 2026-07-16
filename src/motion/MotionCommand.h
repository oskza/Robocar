#ifndef MOTION_COMMAND_H
#define MOTION_COMMAND_H
#include "MotionConfig.h"

enum class MotionCommandType : uint8_t {
    UNKNOWN,
    STATUS,
    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,
    RESET_ODOMETRY,
    STOP,
    BRAKE,
    DRIVE,
    DRIVE_FOR,
    DRIVE_DISTANCE,
    ROTATE_TO,
    ROTATE_BY
};

struct MotionDrivePayload {
    int16_t velocity = 0;
    int16_t turn = 0;
};

struct MotionDriveForPayload {
    int16_t velocity = 0;
    int16_t turn = 0;
    uint32_t durationMs = 0;
};

struct MotionDriveDistancePayload {
    int16_t velocity = 0;
    float distanceMeters = 0.0f;
};

struct MotionRotatePayload {
    uint8_t speed = 0;
    float angleDegrees = 0.0f;
};

union MotionCommandPayload {
    MotionConfig config;
    MotionDrivePayload drive;
    MotionDriveForPayload driveFor;
    MotionDriveDistancePayload driveDistance;
    MotionRotatePayload rotate;
    MotionCommandPayload() {}
};

struct MotionCommand {
    MotionCommandType type = MotionCommandType::UNKNOWN;
    MotionCommandPayload payload{};
};
#endif
