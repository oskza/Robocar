#ifndef MOTION_COMMAND_JSON_READER_H
#define MOTION_COMMAND_JSON_READER_H

#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class MotionCommandJsonReader {
private:
    static bool _readConfig(JsonVariantConst &payload, MotionConfig &cfg);
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};
#endif
