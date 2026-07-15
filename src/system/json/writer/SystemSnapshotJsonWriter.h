#ifndef SYSTEM_SNAPSHOT_JSON_WRITER_H
#define SYSTEM_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../SystemSnapshot.h"

namespace SystemSnapshotJsonWriter {
    void write(JsonObject json, const SystemSnapshot &snapshot);
}
#endif
