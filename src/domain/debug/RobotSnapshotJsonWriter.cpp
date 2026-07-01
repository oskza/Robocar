#include "RobotSnapshotJsonWriter.h"
#include <ArduinoJson.h>

const char *RobotSnapshotJsonWriter::_motionStateToString(MotionState state) {
    switch (state) {
        case MotionState::IDLE:
            return "IDLE";
        case MotionState::MANUAL:
            return "MANUAL";
        case MotionState::TIMED:
            return "TIMED";
        case MotionState::DISTANCE:
            return "DISTANCE";
        case MotionState::ROTATING:
            return "ROTATING";
        default:
            return "UNKNOWN";
    }
}

bool RobotSnapshotJsonWriter::write(const RobotSnapshot &snapshot, char *buffer, size_t size) {
    if (!buffer || size == 0)
        return false;

    JsonDocument doc;

    doc["uptimeMs"] = snapshot.uptimeMs;

    JsonObject network = doc["network"].to<JsonObject>();
    network["connected"] = snapshot.network.connected;
    network["rssi"] = snapshot.network.rssi;
    network["localIp"] = snapshot.network.localIp.toString();

    JsonObject motion = doc["motion"].to<JsonObject>();
    motion["state"] = _motionStateToString(snapshot.motion.state);
    motion["stopped"] = snapshot.motion.stopped;
    motion["currentHeadingDegrees"] = snapshot.motion.currentHeadingDegrees;
    motion["targetHeadingDegrees"] = snapshot.motion.targetHeadingDegrees;
    motion["headingErrorDegrees"] = snapshot.motion.headingErrorDegrees;

    JsonObject drive = doc["drive"].to<JsonObject>();
    drive["leftOutput"] = snapshot.drive.leftOutput;
    drive["rightOutput"] = snapshot.drive.rightOutput;

    JsonObject odometry = doc["odometry"].to<JsonObject>();
    odometry["distanceMeters"] = snapshot.odometry.distanceMeters;
    odometry["averageTicks"] = snapshot.odometry.averageTicks;

    size_t written = serializeJson(doc, buffer, size);

    return written > 0 && written < size;
}
