#ifndef MOTION_COMMAND_H
#define MOTION_COMMAND_H
#include <stdint.h>

enum class MotionCommandType : uint8_t {
    STOP, DRIVE, DRIVE_FOR, DRIVE_DISTANCE,
    ROTATE_TO, ROTATE_BY
};

struct MotionCommand {
    const MotionCommandType type;
    int16_t velocity = 0;
    int16_t turn = 0;
    uint32_t durationMs = 0;
    float distanceMeters = 0.0f;
    float angleDegrees = 0.0f;
    uint8_t speed = 0;

    explicit MotionCommand(MotionCommandType commandType) : type(commandType) {}

    static MotionCommand stop() { return MotionCommand(MotionCommandType::STOP); }

    static MotionCommand drive(int16_t velocity, int16_t turn) {
        MotionCommand cmd(MotionCommandType::DRIVE);
        cmd.velocity = velocity;
        cmd.turn = turn;
        return cmd;
    }

    static MotionCommand driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
        MotionCommand cmd(MotionCommandType::DRIVE_FOR);
        cmd.velocity = velocity;
        cmd.turn = turn;
        cmd.durationMs = durationMs;
        return cmd;
    }

    static MotionCommand driveDistance(int16_t velocity, float distanceMeters) {
        MotionCommand cmd(MotionCommandType::DRIVE_DISTANCE);
        cmd.velocity = velocity;
        cmd.distanceMeters = distanceMeters;
        return cmd;
    }

    static MotionCommand rotateTo(float headingDegrees, uint8_t speed) {
        MotionCommand cmd(MotionCommandType::ROTATE_TO);
        cmd.angleDegrees = headingDegrees;
        cmd.speed = speed;
        return cmd;
    }

    static MotionCommand rotateBy(float angleDegrees, uint8_t speed) {
        MotionCommand cmd(MotionCommandType::ROTATE_BY);
        cmd.angleDegrees = angleDegrees;
        cmd.speed = speed;
        return cmd;
    }
};
#endif
