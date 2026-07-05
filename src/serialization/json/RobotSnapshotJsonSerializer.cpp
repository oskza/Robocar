#include "RobotSnapshotJsonSerializer.h"
#include <ArduinoJson.h>

const char *RobotSnapshotJsonSerializer::_motionStateToString(MotionState state) {
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

bool RobotSnapshotJsonSerializer::write(const RobotSnapshot &snapshot, char *buffer, size_t size) {
    if (!buffer || size == 0)
        return false;

    JsonDocument doc;

    JsonObject system = doc["system"].to<JsonObject>();
    system["uptimeMs"] = snapshot.system.uptimeMs;

    JsonObject heap = system["heap"].to<JsonObject>();
    heap["freeBytes"] = snapshot.system.heap.freeBytes;
    heap["minFreeBytes"] = snapshot.system.heap.minFreeBytes;
    heap["maxAllocBytes"] = snapshot.system.heap.maxAllocBytes;

    JsonObject network = doc["network"].to<JsonObject>();
    network["connected"] = snapshot.network.connected;
    network["rssi"] = snapshot.network.rssi;
    network["localIp"] = snapshot.network.localIp.toString();

    JsonObject power = doc["power"].to<JsonObject>();
    power["connected"] = snapshot.power.connected;
    power["busVoltage"] = snapshot.power.busVoltage;
    power["currentMilliamps"] = snapshot.power.currentMilliamps;
    power["powerMilliwatts"] = snapshot.power.powerMilliwatts;

    JsonObject motion = doc["motion"].to<JsonObject>();
    motion["state"] = _motionStateToString(snapshot.motion.state);
    motion["stopped"] = snapshot.motion.stopped;
    motion["currentHeadingDegrees"] = snapshot.motion.currentHeadingDegrees;
    motion["targetHeadingDegrees"] = snapshot.motion.targetHeadingDegrees;
    motion["headingErrorDegrees"] = snapshot.motion.headingErrorDegrees;
    motion["leftOutput"] = snapshot.motion.leftOutput;
    motion["rightOutput"] = snapshot.motion.rightOutput;

    JsonObject odometry = doc["odometry"].to<JsonObject>();
    odometry["distanceMeters"] = snapshot.odometry.distanceMeters;
    odometry["averageTicks"] = snapshot.odometry.averageTicks;

    size_t written = serializeJson(doc, buffer, size);

    return written > 0 && written < size;
}
