#ifndef ROBOT_COMMAND_HANDLER_H
#define ROBOT_COMMAND_HANDLER_H
#include "../common/RobotCommand.h"
#include "../common/CommandResponse.h"
#include "../../../App.h"

class RobotCommandHandler {
private:
    App &_app;
public:
    explicit RobotCommandHandler(App &app);
    bool execute(
        RobotCommand command,
        const RobotCommandPayload &payload,
        CommandResponse &response
    );
};
#endif
