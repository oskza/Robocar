#ifndef COMMAND_ENVELOPE_H
#define COMMAND_ENVELOPE_H
#include "CommandDomain.h"
#include "RobotCommand.h"
#include "SystemCommand.h"
#include "MotionCommand.h"
#include "WifiCommand.h"

struct CommandEnvelope {
    uint32_t id;
    CommandDomain domain;
    union {
        RobotCommand robot;
        SystemCommand system;
        MotionCommand motion;
        WifiCommand wifi;
    } command;
    union {
        RobotCommandPayload robot;
        SystemCommandPayload system;
        MotionCommandPayload motion;
        WifiCommandPayload wifi;
    } payload;
};
#endif
