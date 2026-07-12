#include "RobotDefaults.h"

void RobotDefaults::applyConfig(RobotConfig &cfg) {
    cfg.telemetryEnabled = true;
    cfg.motionUpdateIntervalMs = 50;
    cfg.wifiUpdateIntervalMs = 3000;
}
