#include "SystemCommandHandler.h"

bool SystemCommandHandler::execute(
    Robot &robot,
    SystemCommand command,
    const SystemCommandPayload &payload,
    CommandResponse &response
) {
    (void)payload;
    switch (command) {
        case SystemCommand::STATUS:
            CommandResponseBuilder::status(response, robot.getSystemSnapshot());
            return true;
        case SystemCommand::PING:
            CommandResponseBuilder::ack(response);
            return true;
        case SystemCommand::RESTART:
            CommandResponseBuilder::ack(response);
            robot.restart();
            return true;
        case SystemCommand::FACTORY_RESET:
            CommandResponseBuilder::ack(response);
            robot.factoryReset();
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
