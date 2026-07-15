#ifndef SYSTEM_SNAPSHOT_JSON_WRITER_H
#define SYSTEM_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../SystemSnapshot.h"

class SystemSnapshotJsonWriter {
public:
    static void write(JsonObject json, const SystemSnapshot &snapshot);
};

#endif
