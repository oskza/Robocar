#ifndef MOTION_COMMAND_H
#define MOTION_COMMAND_H
#include <stdint.h>

enum class MotionCommand : uint8_t {
    STATUS, STOP, DRIVE, DRIVE_FOR,
    DRIVE_DISTANCE, ROTATE_TO, ROTATE_BY
};

struct MotionCommandPayload {
    union {
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
            uint8_t velocity;
            float distanceMeters;
        } driveDistance;
        struct {
            uint8_t speed;
            float angleDegrees;
        } rotate;
    };
};
#endif
