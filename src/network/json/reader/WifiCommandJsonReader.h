#ifndef WIFI_COMMAND_JSON_READER_H
#define WIFI_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../WifiCommand.h"

namespace WifiCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, WifiCommand &out);
}
#endif
