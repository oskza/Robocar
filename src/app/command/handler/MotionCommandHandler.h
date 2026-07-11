#ifndef MOTION_COMMAND_HANDLER_H
#define MOTION_COMMAND_HANDLER_H
#include "../common/MotionCommand.h"
#include "../CommandResponseBuilder.h"
#include "../../../robot/Robot.h"

class MotionCommandHandler {
public:
    static bool execute(
        Robot &robot,
        MotionCommand command,
        const MotionCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
