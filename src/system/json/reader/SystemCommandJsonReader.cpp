#include "SystemCommandJsonReader.h"
#include <string.h>

namespace SystemCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, SystemCommand &out) {
        (void)payload;

        if (command == nullptr)
            return false;

        out = {};

        if (strcmp(command, "restart") == 0) {
            out.type = SystemCommandType::RESTART;
            return true;
        }

        if (strcmp(command, "factoryReset") == 0) {
            out.type = SystemCommandType::FACTORY_RESET;
            return true;
        }

        out.type = SystemCommandType::UNKNOWN;
        return false;
    }
}
