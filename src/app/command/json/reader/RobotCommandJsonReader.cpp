#include "RobotCommandJsonReader.h"

#include <string.h>

bool RobotCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst value = json["command"];

    if (!value.is<const char *>())
        return false;

    const char *cmd = value.as<const char *>();

    command.domain = CommandDomain::ROBOT;

    if (strcmp(cmd, "status") == 0) {
        command.command.robot = RobotCommand::STATUS;
        return true;
    }

    return false;
}
