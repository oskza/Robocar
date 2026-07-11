#ifndef MOTION_SNAPSHOT_JSON_WRITER_H
#define MOTION_SNAPSHOT_JSON_WRITER_H

#include <ArduinoJson.h>
#include "../../../../subsystems/motion/MotionSnapshot.h"

class MotionSnapshotJsonWriter {
private:
    static const char *_stateToString(MotionState state);

public:
    static void write(JsonObject json, const MotionSnapshot &snapshot);
};

#endif
