#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H
#include <stdint.h>

struct RobotConfig {
    uint32_t motionUpdateIntervalMs;
    uint32_t wifiUpdateIntervalMs;
};
#endif
