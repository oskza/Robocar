#ifndef COMMAND_ENVELOPE_H
#define COMMAND_ENVELOPE_H
#include "CommandDomain.h"
#include "RobotCommand.h"
#include "SystemCommand.h"
#include "PowerCommand.h"
#include "MotionCommand.h"
#include "WifiCommand.h"

struct CommandEnvelope {
    uint32_t id;
    CommandDomain domain;
    struct {
        RobotCommand robot;
        SystemCommand system;
        PowerCommand power;
        MotionCommand motion;
        WifiCommand wifi;
    } command;
    struct {
        RobotCommandPayload robot;
        SystemCommandPayload system;
        PowerCommandPayload power;
        MotionCommandPayload motion;
        WifiCommandPayload wifi;
    } payload;
};
#endif
