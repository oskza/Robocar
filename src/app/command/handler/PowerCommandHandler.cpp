#include "PowerCommandHandler.h"

bool PowerCommandHandler::execute(
    Robot &robot,
    PowerCommand command,
    const PowerCommandPayload &payload,
    CommandResponse &response
) {
    (void)payload;
    switch (command) {
        case PowerCommand::STATUS:
            CommandResponseBuilder::status(response, robot.getPowerSnapshot());
            return true;
    }
    CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
    return false;
}
