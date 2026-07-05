#include "RobotCommandHandler.h"

RobotCommandHandler::RobotCommandHandler(App &app) : _app(app) {}

bool RobotCommandHandler::execute(RobotCommand command, const RobotCommandPayload &payload, CommandResponse &response) {
    (void)payload;

    switch (command) {
        case RobotCommand::STATUS:
            response.status = CommandResponseStatus::OK;
            response.type = CommandResponseType::ROBOT_STATUS;
            response.error = CommandError::NONE;
            response.payload.robot = _app.getSnapshot();
            return true;
    }

    response.status = CommandResponseStatus::ERROR;
    response.type = CommandResponseType::ERROR;
    response.error = CommandError::INVALID_COMMAND;
    return false;
}
