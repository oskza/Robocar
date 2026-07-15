#ifndef ROBOT_SNAPSHOT_JSON_WRITER_H
#define ROBOT_SNAPSHOT_JSON_WRITER_H

#include <ArduinoJson.h>
#include "robot/RobotSnapshot.h"

class RobotSnapshotJsonWriter {
public:
    static void write(JsonObject json, const RobotSnapshot &snapshot);
};

#endif
