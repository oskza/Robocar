#ifndef ROBOT_SNAPSHOT_JSON_WRITER_H
#define ROBOT_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../RobotSnapshot.h"

namespace RobotSnapshotJsonWriter {
    void write(JsonObject json, const RobotSnapshot &snapshot);
}
#endif
