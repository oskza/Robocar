#include "PowerCommandJsonReader.h"

#include <string.h>

bool PowerCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst commandValue = json["command"];

    if (!commandValue.is<const char *>())
        return false;

    const char *cmd = commandValue.as<const char *>();

    command.domain = CommandDomain::POWER;

    if (strcmp(cmd, "status") == 0) {
        command.command.power = PowerCommand::STATUS;
        return true;
    }

    return false;
}
