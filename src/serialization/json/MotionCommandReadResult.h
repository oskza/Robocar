#ifndef MOTION_COMMAND_READ_RESULT_H
#define MOTION_COMMAND_READ_RESULT_H
#include "../../domain/motion/MotionCommand.h"

struct MotionCommandReadResult {
    bool ok;
    MotionCommand command;
    static MotionCommandReadResult success(const MotionCommand &command) {
        return { true, command };
    }
    static MotionCommandReadResult failure() {
        return { false, MotionCommand::stop() };
    }
};
#endif
