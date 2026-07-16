#ifndef POWER_SNAPSHOT_JSON_WRITER_H
#define POWER_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../PowerSnapshot.h"

namespace PowerSnapshotJsonWriter {
    void write(JsonObject json, const PowerSnapshot &snapshot);
};
#endif
