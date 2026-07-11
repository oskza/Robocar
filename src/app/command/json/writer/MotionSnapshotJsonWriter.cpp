#include "MotionSnapshotJsonWriter.h"

const char *MotionSnapshotJsonWriter::_stateToString(MotionState state) {
    switch (state) {
        case MotionState::IDLE: return "idle";
        case MotionState::MANUAL: return "manual";
        case MotionState::TIMED: return "timed";
        case MotionState::DISTANCE: return "distance";
        case MotionState::ROTATING: return "rotating";
    }
    return "unknown";
}

void MotionSnapshotJsonWriter::write(JsonObject json, const MotionSnapshot &snapshot) {
    json["state"] = _stateToString(snapshot.state);
    json["stopped"] = snapshot.stopped;

    JsonObject output = json["output"].to<JsonObject>();
    output["left"] = snapshot.output.left;
    output["right"] = snapshot.output.right;

    JsonObject heading = json["heading"].to<JsonObject>();
    heading["currentDegrees"] = snapshot.heading.currentDegrees;
    if (snapshot.state != MotionState::ROTATING)
        return;
    heading["targetDegrees"] = snapshot.heading.targetDegrees;
    heading["errorDegrees"] = snapshot.heading.errorDegrees;

    JsonObject odometry = json["odometry"].to<JsonObject>();
    odometry["distanceMeters"] = snapshot.odometry.distanceMeters;
    odometry["averageTicks"] = snapshot.odometry.averageTicks;
}
