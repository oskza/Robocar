#ifndef MOTION_SNAPSHOT_JSON_WRITER_H
#define MOTION_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../MotionSnapshot.h"

namespace MotionSnapshotJsonWriter {
    void write(JsonObject json, const MotionSnapshot &snapshot);
}
#endif
