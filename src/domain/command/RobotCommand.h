#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H
#include <stdint.h>

enum class RobotCommandType : uint8_t {
    STOP, DRIVE, DRIVE_FOR, DRIVE_DISTANCE,
    ROTATE_TO, ROTATE_BY
};

struct RobotCommand {
    const RobotCommandType type;
    int16_t velocity = 0;
    int16_t turn = 0;
    uint32_t durationMs = 0;
    float distanceMeters = 0.0f;
    float angleDegrees = 0.0f;
    uint8_t speed = 0;

    explicit RobotCommand(RobotCommandType commandType) : type(commandType) {}

    static RobotCommand stop() { return RobotCommand(RobotCommandType::STOP); }

    static RobotCommand drive(int16_t velocity, int16_t turn) {
        RobotCommand cmd(RobotCommandType::DRIVE);
        cmd.velocity = velocity;
        cmd.turn = turn;
        return cmd;
    }

    static RobotCommand driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
        RobotCommand cmd(RobotCommandType::DRIVE_FOR);
        cmd.velocity = velocity;
        cmd.turn = turn;
        cmd.durationMs = durationMs;
        return cmd;
    }

    static RobotCommand driveDistance(int16_t velocity, float distanceMeters) {
        RobotCommand cmd(RobotCommandType::DRIVE_DISTANCE);
        cmd.velocity = velocity;
        cmd.distanceMeters = distanceMeters;
        return cmd;
    }

    static RobotCommand rotateTo(float headingDegrees, uint8_t speed) {
        RobotCommand cmd(RobotCommandType::ROTATE_TO);
        cmd.angleDegrees = headingDegrees;
        cmd.speed = speed;
        return cmd;
    }

    static RobotCommand rotateBy(float angleDegrees, uint8_t speed) {
        RobotCommand cmd(RobotCommandType::ROTATE_BY);
        cmd.angleDegrees = angleDegrees;
        cmd.speed = speed;
        return cmd;
    }
};

#endif
