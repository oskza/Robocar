#ifndef WIFI_CONFIG_JSON_WRITER_H
#define WIFI_CONFIG_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../WifiConfig.h"

namespace WifiConfigJsonWriter {
    void write(JsonObject json, const WifiConfig &config);
}
#endif
