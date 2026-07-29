#ifndef MOTION_COMMAND_HANDLER_H
#define MOTION_COMMAND_HANDLER_H
#include "MotionCommand.h"
#include "MotionController.h"
#include "MotionStorage.h"
#include "command/CommandResponse.h"

class MotionCommandHandler {
private:
    MotionController &_motion;
    MotionStorage &_storage;
public:
    MotionCommandHandler(MotionController &motion, MotionStorage &storage);
    bool execute(const MotionCommand &command, CommandResponse &response);
};
#endif
