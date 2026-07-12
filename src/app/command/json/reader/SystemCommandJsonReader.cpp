#include "SystemCommandJsonReader.h"

#include <string.h>

bool SystemCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst value = json["command"];
    if (!value.is<const char *>())
        return false;
    const char *cmd = value.as<const char *>();
    command.domain = CommandDomain::SYSTEM;
    if (strcmp(cmd, "status") == 0) {
        command.command.system = SystemCommand::STATUS;
        return true;
    }
    if (strcmp(cmd, "ping") == 0) {
        command.command.system = SystemCommand::PING;
        return true;
    }
    if (strcmp(cmd, "restart") == 0) {
        command.command.system = SystemCommand::RESTART;
        return true;
    }
    if (strcmp(cmd, "factoryReset") == 0) {
        command.command.system = SystemCommand::FACTORY_RESET;
        return true;
    }
    return false;
}
