#ifndef POWER_SNAPSHOT_JSON_WRITER_H
#define POWER_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "domain/power/PowerSnapshot.h"

class PowerSnapshotJsonWriter {
public:
    static void write(JsonObject json, const PowerSnapshot &snapshot);
};
#endif
