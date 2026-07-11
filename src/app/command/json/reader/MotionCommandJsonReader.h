#ifndef MOTION_COMMAND_JSON_READER_H
#define MOTION_COMMAND_JSON_READER_H

#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class MotionCommandJsonReader {
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};

#endif
