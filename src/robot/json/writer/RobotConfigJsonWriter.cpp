#include "RobotConfigJsonWriter.h"

namespace RobotConfigJsonWriter {
    void write(JsonObject json, const RobotConfig &config) {
        json["telemetryEnabled"] = config.telemetryEnabled;
        json["motionUpdateIntervalMs"] = config.motionUpdateIntervalMs;
        json["wifiUpdateIntervalMs"] = config.wifiUpdateIntervalMs;
    }
}
