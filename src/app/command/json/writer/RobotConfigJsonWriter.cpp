#include "RobotConfigJsonWriter.h"

void RobotConfigJsonWriter::write(JsonObject json, const RobotConfig &cfg) {
    json["telemetryEnabled"] = cfg.telemetryEnabled;
    json["motionUpdateIntervalMs"] = cfg.motionUpdateIntervalMs;
    json["wifiUpdateIntervalMs"] = cfg.wifiUpdateIntervalMs;
}
