#include "SystemCommandHandler.h"
#include "command/CommandPostAction.h"
#include "command/CommandResponseBuilder.h"

namespace SystemCommandHandler {
    bool execute(SystemCommand command, const SystemCommandPayload &payload, CommandResponse &response) {
        (void)payload;
        switch (command.type) {
            case SystemCommandType::RESTART:
                CommandResponseBuilder::ack(response, CommandPostAction::RESTART);
                return true;
            case SystemCommandType::FACTORY_RESET:
                CommandResponseBuilder::ack(response, CommandPostAction::FACTORY_RESET);
                return true;
        }
        CommandResponseBuilder::error(response, CommandError::INVALID_COMMAND);
        return false;
    }
}
