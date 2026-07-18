#include "RobotDefaults.h"
namespace RobotDefaults {
    RobotConfig config() {
        RobotConfig config{};
        config.telemetryEnabled = true;
        config.motionUpdateIntervalMs = 50;
        config.wifiUpdateIntervalMs = 100;
        return config;
    }
}
