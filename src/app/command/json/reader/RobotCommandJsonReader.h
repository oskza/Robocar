#ifndef ROBOT_COMMAND_JSON_READER_H
#define ROBOT_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class RobotCommandJsonReader {
private:
    static bool _readConfig(JsonVariantConst &payload, RobotConfig &cfg);
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};
#endif
