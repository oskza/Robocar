#ifndef WIFI_COMMAND_JSON_READER_H
#define WIFI_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "command/CommandEnvelope.h"

class WifiCommandJsonReader {
private:
    static bool _readConfig(JsonObjectConst json, WifiConfig &cfg);
    static bool _readCredentials(JsonObjectConst json, WifiCredentials &creds);
public:
    static bool read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command);
};
#endif
