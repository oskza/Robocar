#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H
#include <stdint.h>

struct RobotConfig {
    bool telemetryEnabled = false;
    uint32_t motionUpdateIntervalMs = 0;
    uint32_t wifiUpdateIntervalMs = 0;
};
#endif
