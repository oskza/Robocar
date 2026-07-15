#ifndef ROBOT_COMMAND_JSON_READER_H
#define ROBOT_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "command/CommandEnvelope.h"

class RobotCommandJsonReader {
private:
    static bool _readConfig(JsonObjectConst json, RobotConfig &cfg);
public:
    static bool read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command);
};
#endif
