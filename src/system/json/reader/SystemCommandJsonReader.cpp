#include "SystemCommandJsonReader.h"
#include <string.h>

namespace SystemCommandJsonReader {
    bool read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command) {
        (void)payload;
        command.domain = CommandDomain::SYSTEM;
        if (strcmp(commandName, "restart") == 0) {
            command.command.system = SystemCommand::RESTART;
            return true;
        }
        if (strcmp(commandName, "factoryReset") == 0) {
            command.command.system = SystemCommand::FACTORY_RESET;
            return true;
        }
        return false;
    }
}
