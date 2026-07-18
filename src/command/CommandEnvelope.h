#ifndef COMMAND_ENVELOPE_H
#define COMMAND_ENVELOPE_H
#include "CommandDomain.h"
#include "motion/MotionCommand.h"
#include "network/WifiCommand.h"
#include "robot/RobotCommand.h"
#include "system/SystemCommand.h"

struct CommandEnvelope {
    uint32_t id = 0;
    CommandDomain domain = CommandDomain::UNKNOWN;
    RobotCommand robot{};
    SystemCommand system{};
    MotionCommand motion{};
    WifiCommand wifi{};
};
#endif
