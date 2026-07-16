#ifndef MOTION_COMMAND_HANDLER_H
#define MOTION_COMMAND_HANDLER_H
#include "MotionCommand.h"
#include "MotionController.h"
#include "MotionStorage.h"
#include "command/CommandResponse.h"

namespace MotionCommandHandler {
    bool execute(
        MotionController &motion,
        MotionStorage &storage,
        const MotionCommand &command,
        CommandResponse &response
    );
}
#endif
