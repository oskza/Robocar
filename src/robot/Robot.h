#ifndef ROBOT_H
#define ROBOT_H
#include "RobotConfig.h"
#include "RobotSnapshot.h"
#include "motion/MotionController.h"
#include "motion/MotionConfig.h"
#include "network/WifiController.h"
#include "network/WifiConfig.h"
#include "network/WifiCredentials.h"
#include "power/PowerService.h"

class Robot {
private:
    PowerService &_power;
    WifiController &_wifi;
    MotionController &_motion;
    RobotConfig _cfg;
public:
    Robot(PowerService &power, WifiController &wifi, MotionController &motion);
    bool begin(
        const RobotConfig &robotConfig,
        const MotionConfig &motionConfig,
        const WifiConfig &wifiConfig,
        const WifiCredentials &stationCredentials,
        const WifiCredentials &accessPointCredentials
    );
    RobotSnapshot getSnapshot();
    void getConfig(RobotConfig &cfg) const;
    void setConfig(const RobotConfig &cfg);
    const char *getHostname() const;
    bool isTelemetryEnabled() const;
};
#endif
