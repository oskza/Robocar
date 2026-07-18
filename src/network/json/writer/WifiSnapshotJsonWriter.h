#ifndef WIFI_SNAPSHOT_JSON_WRITER_H
#define WIFI_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../WifiSnapshot.h"

namespace WifiSnapshotJsonWriter {
    void write(JsonObject json, const WifiSnapshot &snapshot);
}
#endif
