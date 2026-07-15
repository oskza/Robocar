#include "RobotCommandJsonReader.h"
#include <string.h>
#include "json/JsonValueReader.h"

bool RobotCommandJsonReader::_readConfig(JsonVariantConst &payload, RobotConfig &cfg) {
    return JsonValueReader::readBool(payload["telemetryEnabled"], cfg.telemetryEnabled)
        && JsonValueReader::readUint32(payload["motionUpdateIntervalMs"], cfg.motionUpdateIntervalMs)
        && JsonValueReader::readUint32(payload["wifiUpdateIntervalMs"], cfg.wifiUpdateIntervalMs);
}

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
    if (strcmp(cmd, "getConfig") == 0) {
        command.command.robot = RobotCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(cmd, "setConfig") == 0) {
        command.command.robot = RobotCommand::SET_CONFIG;
        JsonVariantConst payload = json["payload"];
        RobotConfig &cfg = command.payload.robot.cfg;
        return _readConfig(payload, cfg);
    }
    if (strcmp(cmd, "reset") == 0) {
        command.command.robot = RobotCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(cmd, "enableTelemetry") == 0) {
        command.command.robot = RobotCommand::ENABLE_TELEMETRY;
        return true;
    }
    if (strcmp(cmd, "disableTelemetry") == 0) {
        command.command.robot = RobotCommand::DISABLE_TELEMETRY;
        return true;
    }
    return false;
}
