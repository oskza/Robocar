#include "CommandJsonParser.h"
#include <ArduinoJson.h>
#include <string.h>
#include "command/CommandDomainStrings.h"
#include "motion/json/reader/MotionCommandJsonReader.h"
#include "network/json/reader/WifiCommandJsonReader.h"
#include "robot/json/reader/RobotCommandJsonReader.h"
#include "system/json/reader/SystemCommandJsonReader.h"

namespace {
    const char *getRequiredString(JsonObjectConst object, const char *key) {
        const JsonVariantConst value = object[key];
        return (value.is<const char*>())
            ? value.as<const char*>()
            : nullptr;
    }

    bool getOptionalId(JsonObjectConst root, uint32_t &id) {
        const JsonVariantConst value = root["id"];
        if (value.isNull()) {
            id = 0;
            return true;
        }
        if (!value.is<uint32_t>())
            return false;
        id = value.as<uint32_t>();
        return true;
    }

    bool getOptionalPayload(JsonObjectConst root, JsonObjectConst &payload) {
        const JsonVariantConst value = root["payload"];
        if (value.isNull()) {
            payload = JsonObjectConst{};
            return true;
        }
        if (!value.is<JsonObjectConst>())
            return false;
        payload = value.as<JsonObjectConst>();
        return true;
    }

    CommandError parseCommand(
        CommandDomain domain,
        const char *commandType,
        JsonObjectConst payload,
        CommandEnvelope &envelope
    ) {
        switch (domain) {
            case CommandDomain::ROBOT:
                return RobotCommandJsonReader::read(commandType, payload, envelope.robot)
                    ? CommandError::NONE
                    : CommandError::INVALID_COMMAND;
            case CommandDomain::SYSTEM:
                return SystemCommandJsonReader::read(commandType, payload, envelope.system)
                    ? CommandError::NONE
                    : CommandError::INVALID_COMMAND;
            case CommandDomain::MOTION:
                return MotionCommandJsonReader::read(commandType, payload, envelope.motion)
                    ? CommandError::NONE
                    : CommandError::INVALID_COMMAND;
            case CommandDomain::WIFI:
                return WifiCommandJsonReader::read(commandType, payload, envelope.wifi)
                    ? CommandError::NONE
                    : CommandError::INVALID_COMMAND;
            case CommandDomain::UNKNOWN:
                return CommandError::INVALID_COMMAND;
        }
        return CommandError::INVALID_COMMAND;
    }
}

namespace CommandJsonParser {
    CommandError parse(const char *data, size_t length, CommandEnvelope &command) {
        if (data == nullptr || length == 0)
            return CommandError::INVALID_JSON;
        JsonDocument document;
        if (deserializeJson(document, data, length) != DeserializationError::Ok)
            return CommandError::INVALID_JSON;
        const JsonObjectConst root = document.as<JsonObjectConst>();
        if (root.isNull())
            return CommandError::INVALID_ENVELOPE;
        const char *messageType = getRequiredString(root, "type");
        if (messageType == nullptr || strcmp(messageType, "cmd") != 0)
            return CommandError::INVALID_ENVELOPE;
        const char *domainText = getRequiredString(root, "domain");
        if (domainText == nullptr)
            return CommandError::INVALID_ENVELOPE;
        const char *commandType = getRequiredString(root, "command");
        if (commandType == nullptr)
            return CommandError::INVALID_ENVELOPE;
        CommandDomain domain = CommandDomain::UNKNOWN;
        if (!CommandDomainStrings::fromString(domainText, domain))
            return CommandError::INVALID_COMMAND;
        CommandEnvelope parsedCommand{};
        parsedCommand.domain = domain;
        if (!getOptionalId(root, parsedCommand.id))
            return CommandError::INVALID_ENVELOPE;
        JsonObjectConst payload{};
        if (!getOptionalPayload(root, payload))
            return CommandError::INVALID_PAYLOAD;
        const CommandError error = parseCommand(domain, commandType, payload, parsedCommand);
        if (error != CommandError::NONE)
            return error;
        command = parsedCommand;
        return CommandError::NONE;
    }
}
