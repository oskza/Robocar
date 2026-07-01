#ifndef ROBOT_SNAPSHOT_JSON_WRITER_H
#define ROBOT_SNAPSHOT_JSON_WRITER_H
#include <Arduino.h>
#include "RobotSnapshot.h"

class RobotSnapshotJsonWriter {
private:
    static const char *_motionStateToString(MotionState state);
public:
    static bool write(const RobotSnapshot &snapshot, char *buffer, size_t size);
};
#endif
