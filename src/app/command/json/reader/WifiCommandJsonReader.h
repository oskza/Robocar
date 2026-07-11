#ifndef WIFI_COMMAND_JSON_READER_H
#define WIFI_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class WifiCommandJsonReader {
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};
#endif
