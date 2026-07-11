#include "RobotConfigJsonWriter.h"

void RobotConfigJsonWriter::write(JsonObject json, const RobotConfig &config) {
    json["motionUpdateIntervalMs"] = config.motionUpdateIntervalMs;
    json["wifiUpdateIntervalMs"] = config.wifiUpdateIntervalMs;
}
