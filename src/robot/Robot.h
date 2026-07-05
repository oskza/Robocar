#ifndef ROBOT_H
#define ROBOT_H
#include "subsystems/system/SystemController.h"
#include "subsystems/power/PowerController.h"
#include "subsystems/network/WifiController.h"
#include "subsystems/network/WifiStorage.h"
#include "subsystems/motion/MotionController.h"
#include "subsystems/motion/MotionStorage.h"
#include "RobotStorage.h"
#include "RobotSnapshot.h"

class Robot {
private:
    SystemController &_system;
    PowerController &_power;
    WifiController &_wifi;
    WifiStorage &_wifiStorage;
    MotionController &_motion;
    MotionStorage &_motionStorage;
    RobotStorage &_storage;
    RobotConfig _cfg;
    uint32_t _lastMotionUpdateMs;
    uint32_t _lastWifiUpdateMs;
public:
    Robot(
        SystemController &system,
        PowerController &power,
        WifiController &wifi,
        WifiStorage &wifiStorage,
        MotionController &motion,
        MotionStorage &motionStorage,
        RobotStorage &storage
    );
    void begin(
        uint32_t motorPwmFrequency,
        uint8_t encoderSlots,
        float powerMaxCurrentAmps,
        float powerShuntOhms
    );
    void update();
    RobotSnapshot getSnapshot() const;
};
#endif
