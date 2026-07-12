#ifndef WIFI_COMMAND_JSON_READER_H
#define WIFI_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class WifiCommandJsonReader {
private:
    static bool _readConfig(JsonVariantConst &payload, WifiConfig &cfg);
    static bool _readCredentials(JsonVariantConst &payload, WifiCredentials &creds);
public:
    static bool read(JsonObjectConst json, CommandEnvelope &command);
};
#endif
