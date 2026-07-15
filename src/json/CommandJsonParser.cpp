#include "CommandJsonParser.h"
#include <ArduinoJson.h>
#include <string.h>
#include "robot/json/reader/RobotCommandJsonReader.h"
#include "system/json/reader/SystemCommandJsonReader.h"
#include "motion/json/reader/MotionCommandJsonReader.h"
#include "network/json/reader/WifiCommandJsonReader.h"

bool CommandJsonParser::parse(const char *data, size_t length, CommandEnvelope &command) {
    if (!data || length == 0)
        return false;

    JsonDocument doc;
    if (deserializeJson(doc, data, length))
        return false;

    JsonObjectConst root = doc.as<JsonObjectConst>();
    JsonVariantConst typeValue = root["type"];
    if (!(typeValue.is<const char*>() && strcmp(typeValue.as<const char*>(), "cmd") == 0))
        return false;

    command = {};
    if (root["id"].is<uint32_t>())
        command.id = root["id"].as<uint32_t>();

    JsonVariantConst domainValue = root["domain"];
    if (!domainValue.is<const char*>())
        return false;

    const char *domain = domainValue.as<const char *>();
    if (strcmp(domain, "robot") == 0)
        return RobotCommandJsonReader::read(root, command);
    if (strcmp(domain, "system") == 0)
        return SystemCommandJsonReader::read(root, command);
    if (strcmp(domain, "motion") == 0)
        return MotionCommandJsonReader::read(root, command);
    if (strcmp(domain, "wifi") == 0)
        return WifiCommandJsonReader::read(root, command);
    return false;
}
