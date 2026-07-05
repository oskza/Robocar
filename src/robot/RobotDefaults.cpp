#include "RobotDefaults.h"

void RobotDefaults::applyConfig(RobotConfig &cfg) {
    cfg.motionUpdateIntervalMs = 50;
    cfg.wifiUpdateIntervalMs = 3000;
}
