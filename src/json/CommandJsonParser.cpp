#include "CommandJsonParser.h"
#include <ArduinoJson.h>
#include <string.h>
#include "motion/json/reader/MotionCommandJsonReader.h"
#include "network/json/reader/WifiCommandJsonReader.h"
#include "robot/json/reader/RobotCommandJsonReader.h"
#include "system/json/reader/SystemCommandJsonReader.h"

namespace {
    constexpr const char *KEY_ID = "id";
    constexpr const char *KEY_TYPE = "type";
    constexpr const char *KEY_DOMAIN = "domain";
    constexpr const char *KEY_COMMAND = "command";
    constexpr const char *KEY_PAYLOAD = "payload";

    constexpr const char *TYPE_COMMAND = "cmd";

    constexpr const char *DOMAIN_ROBOT = "robot";
    constexpr const char *DOMAIN_SYSTEM = "system";
    constexpr const char *DOMAIN_MOTION = "motion";
    constexpr const char *DOMAIN_WIFI = "wifi";

    const char *readRequiredString(JsonObjectConst object, const char *key) {
        const JsonVariantConst value = object[key];
        if (!value.is<const char *>())
            return nullptr;
        return value.as<const char *>();
    }

    bool readOptionalPayload(JsonObjectConst root, JsonObjectConst &payload) {
        const JsonVariantConst value = root[KEY_PAYLOAD];
        if (value.isNull()) {
            payload = JsonObjectConst{};
            return true;
        }
        if (!value.is<JsonObjectConst>())
            return false;
        payload = value.as<JsonObjectConst>();
        return true;
    }

    bool routeCommand(const char *domain, const char *commandName, JsonObjectConst payload, CommandEnvelope &command) {
        if (strcmp(domain, DOMAIN_ROBOT) == 0)
            return RobotCommandJsonReader::read(commandName, payload, command);
        if (strcmp(domain, DOMAIN_SYSTEM) == 0)
            return SystemCommandJsonReader::read(commandName, payload, command);
        if (strcmp(domain, DOMAIN_MOTION) == 0)
            return MotionCommandJsonReader::read(commandName, payload, command);
        if (strcmp(domain, DOMAIN_WIFI) == 0)
            return WifiCommandJsonReader::read(commandName, payload, command);
        return false;
    }
}

namespace CommandJsonParser {
    bool parse(const char *data, size_t length, CommandEnvelope &command) {
        if (data == nullptr || length == 0)
            return false;

        JsonDocument document;
        if (deserializeJson(document, data, length) != DeserializationError::Ok)
            return false;

        const JsonObjectConst root = document.as<JsonObjectConst>();
        if (root.isNull())
            return false;

        const char *type = readRequiredString(root, KEY_TYPE);
        if (type == nullptr || strcmp(type, TYPE_COMMAND) != 0)
            return false;

        const char *domain = readRequiredString(root, KEY_DOMAIN);
        if (domain == nullptr)
            return false;

        const char *commandName = readRequiredString(root, KEY_COMMAND);
        if (commandName == nullptr)
            return false;

        JsonObjectConst payload{};
        if (!readOptionalPayload(root, payload))
            return false;

        CommandEnvelope parsedCommand{};

        const JsonVariantConst idValue = root[KEY_ID];
        if (!idValue.isNull()) {
            if (!idValue.is<uint32_t>())
                return false;
            parsedCommand.id = idValue.as<uint32_t>();
        }

        if (!routeCommand(domain, commandName, payload, parsedCommand))
            return false;

        command = parsedCommand;
        return true;
    }
}
