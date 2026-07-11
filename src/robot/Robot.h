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
    bool getConfig(RobotConfig &cfg) const;
    bool setConfig(const RobotConfig &cfg);
    bool resetConfig();

    SystemSnapshot getSystemSnapshot() const;
    uint32_t getUptimeMs() const;
    void restart();
    void factoryReset();

    PowerSnapshot getPowerSnapshot() const;

    MotionSnapshot getMotionSnapshot() const;
    bool getMotionConfig(MotionConfig &cfg) const;
    bool setMotionConfig(const MotionConfig &cfg);
    bool resetMotion();
    void stop();
    void brake();
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void rotateTo(float headingDegrees, uint8_t speed);
    void rotateBy(float degrees, uint8_t speed);

    WifiSnapshot getWifiSnapshot() const;
    bool getWifiConfig(WifiConfig &cfg) const;
    bool setWifiConfig(const WifiConfig &cfg);
    bool resetWifiConfig();
    bool setStationCredentials(const WifiCredentials &credentials);
    bool resetStationCredentials();
    bool getAccessPointCredentials(WifiCredentials &credentials) const;
    bool setAccessPointCredentials(const WifiCredentials &credentials);
    bool resetAccessPointCredentials();
    bool resetWifi();
};
#endif
