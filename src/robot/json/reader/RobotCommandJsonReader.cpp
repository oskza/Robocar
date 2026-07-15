#include "RobotCommandJsonReader.h"
#include <string.h>
#include "json/JsonValueReader.h"

bool RobotCommandJsonReader::_readConfig(JsonObjectConst json, RobotConfig &cfg) {
    return JsonValueReader::readBool(json["telemetryEnabled"], cfg.telemetryEnabled)
        && JsonValueReader::readUint32(json["motionUpdateIntervalMs"], cfg.motionUpdateIntervalMs)
        && JsonValueReader::readUint32(json["wifiUpdateIntervalMs"], cfg.wifiUpdateIntervalMs);
}

bool RobotCommandJsonReader::read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command) {
    if (commandName == nullptr)
        return false;
    command.domain = CommandDomain::ROBOT;
    if (strcmp(commandName, "status") == 0) {
        command.command.robot = RobotCommand::STATUS;
        return true;
    }
    if (strcmp(commandName, "getConfig") == 0) {
        command.command.robot = RobotCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "setConfig") == 0) {
        command.command.robot = RobotCommand::SET_CONFIG;
        RobotConfig &cfg = command.payload.robot.cfg;
        return _readConfig(payload, cfg);
    }
    if (strcmp(commandName, "reset") == 0) {
        command.command.robot = RobotCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "enableTelemetry") == 0) {
        command.command.robot = RobotCommand::ENABLE_TELEMETRY;
        return true;
    }
    if (strcmp(commandName, "disableTelemetry") == 0) {
        command.command.robot = RobotCommand::DISABLE_TELEMETRY;
        return true;
    }
    return false;
}
