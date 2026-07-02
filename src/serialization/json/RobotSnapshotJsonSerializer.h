#ifndef ROBOT_SNAPSHOT_JSON_SERIALIZER_H
#define ROBOT_SNAPSHOT_JSON_SERIALIZER_H
#include <Arduino.h>
#include "../../domain/RobotSnapshot.h"

class RobotSnapshotJsonSerializer {
private:
    static const char *_motionStateToString(MotionState state);
public:
    static bool write(const RobotSnapshot &snapshot, char *buffer, size_t size);
};
#endif
