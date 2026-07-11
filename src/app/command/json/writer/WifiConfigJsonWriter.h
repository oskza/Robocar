#ifndef WIFI_CONFIG_JSON_WRITER_H
#define WIFI_CONFIG_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../../../subsystems/network/WifiConfig.h"
#include "../../../../subsystems/network/WifiModeStrings.h"

class WifiConfigJsonWriter {
public:
    static void write(JsonObject json, const WifiConfig &config);
};
#endif
