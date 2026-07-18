#include "RobotCommandJsonReader.h"
#include <string.h>
#include <JsonValueReader.h>

namespace {
    bool readConfig(JsonObjectConst json, RobotConfig &config) {
        return JsonValueReader::readBool(json["telemetryEnabled"], config.telemetryEnabled)
            && JsonValueReader::readUint32(json["motionUpdateIntervalMs"], config.motionUpdateIntervalMs)
            && JsonValueReader::readUint32(json["wifiUpdateIntervalMs"], config.wifiUpdateIntervalMs);
    }
}

namespace RobotCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, RobotCommand &out) {
        if (command == nullptr)
            return false;
        out.type = RobotCommandType::UNKNOWN;
        if (strcmp(command, "status") == 0) {
            out.type = RobotCommandType::STATUS;
            return true;
        }
        if (strcmp(command, "getConfig") == 0) {
            out.type = RobotCommandType::GET_CONFIG;
            return true;
        }
        if (strcmp(command, "setConfig") == 0) {
            out.type = RobotCommandType::SET_CONFIG;
            return readConfig(payload, out.payload.config);
        }
        if (strcmp(command, "resetConfig") == 0) {
            out.type = RobotCommandType::RESET_CONFIG;
            return true;
        }
        if (strcmp(command, "enableTelemetry") == 0) {
            out.type = RobotCommandType::ENABLE_TELEMETRY;
            return true;
        }
        if (strcmp(command, "disableTelemetry") == 0) {
            out.type = RobotCommandType::DISABLE_TELEMETRY;
            return true;
        }
        return false;
    }
}
