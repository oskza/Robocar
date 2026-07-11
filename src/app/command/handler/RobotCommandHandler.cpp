#include "RobotCommandHandler.h"

bool RobotCommandHandler::execute(
    Robot &robot,
    RobotCommand command,
    const RobotCommandPayload &payload,
    CommandResponse &response
) {
    (void)payload;
    switch (command) {
        case RobotCommand::STATUS:
            CommandResponseBuilder::status(response, robot.getSnapshot());
            return true;
        case RobotCommand::GET_CONFIG: {
            RobotConfig cfg{};
            robot.getConfig(cfg);
            CommandResponseBuilder::config(response, cfg);
            return true;
        }
        case RobotCommand::SET_CONFIG:
            if (!robot.setConfig(payload.cfg)) {
                CommandResponseBuilder::error(response, CommandError::STORAGE_ERROR);
                return false;
            }
            CommandResponseBuilder::ack(response);
            return true;
        case RobotCommand::RESET_CONFIG:
            robot.resetConfig();
            CommandResponseBuilder::ack(response);
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
