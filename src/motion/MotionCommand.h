#ifndef MOTION_COMMAND_H
#define MOTION_COMMAND_H
#include "motion/MotionConfig.h"

enum class MotionCommand : uint8_t {
    STATUS,
    GET_CONFIG,
    SET_CONFIG,
    RESET_CONFIG,
    STOP,
    BRAKE,
    DRIVE,
    DRIVE_FOR,
    DRIVE_DISTANCE,
    ROTATE_TO,
    ROTATE_BY
};

struct MotionCommandPayload {
    union {
        MotionConfig cfg;
        struct {
            int16_t velocity;
            int16_t turn;
        } drive;
        struct {
            int16_t velocity;
            int16_t turn;
            uint32_t durationMs;
        } driveFor;
        struct {
            int16_t velocity;
            float distanceMeters;
        } driveDistance;
        struct {
            uint8_t speed;
            float angleDegrees;
        } rotate;
    };
};
#endif
