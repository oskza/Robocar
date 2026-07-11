#ifndef POWER_COMMAND_JSON_READER_H
#define POWER_COMMAND_JSON_READER_H

#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class PowerCommandJsonReader {
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};

#endif
