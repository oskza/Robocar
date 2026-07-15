#ifndef COMMAND_ENVELOPE_H
#define COMMAND_ENVELOPE_H
#include "CommandDomain.h"
#include "robot/RobotCommand.h"
#include "system/SystemCommand.h"
#include "motion/MotionCommand.h"
#include "network/WifiCommand.h"

struct CommandEnvelope {
    uint32_t id;
    CommandDomain domain;
    struct {
        RobotCommand robot;
        SystemCommand system;
        MotionCommand motion;
        WifiCommand wifi;
    } command;
    struct {
        RobotCommandPayload robot;
        SystemCommandPayload system;
        MotionCommandPayload motion;
        WifiCommandPayload wifi;
    } payload;
};
#endif
