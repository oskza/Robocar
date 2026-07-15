#ifndef MOTION_COMMAND_JSON_READER_H
#define MOTION_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "command/CommandEnvelope.h"

class MotionCommandJsonReader {
private:
    static bool _readConfig(JsonObjectConst json, MotionConfig &cfg);
public:
    static bool read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command);
};
#endif
