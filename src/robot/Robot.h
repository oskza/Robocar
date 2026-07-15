#ifndef ROBOT_H
#define ROBOT_H
#include "power/PowerService.h"
#include "network/WifiController.h"
#include "network/WifiStorage.h"
#include "motion/MotionController.h"
#include "motion/MotionStorage.h"
#include "RobotStorage.h"
#include "RobotSnapshot.h"

class Robot {
private:
    PowerService &_power;
    WifiController &_wifi;
    MotionController &_motion;
    WifiStorage &_wifiStorage;
    MotionStorage &_motionStorage;
    RobotStorage &_storage;
    RobotConfig _cfg;
    uint32_t _lastMotionUpdateMs;
    uint32_t _lastWifiUpdateMs;
    bool _setTelemetryEnabled(bool enabled = true);
public:
    Robot(
        PowerService &power,
        WifiController &wifi,
        MotionController &motion,
        WifiStorage &wifiStorage,
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

    bool enableTelemetry();
    bool disableTelemetry();
    bool isTelemetryEnabled();

    RobotSnapshot getSnapshot() const;
    void getConfig(RobotConfig &cfg) const;
    bool setConfig(const RobotConfig &cfg);
    void resetConfig();

    SystemSnapshot getSystemSnapshot() const;
    uint32_t getUptimeMs() const;
    void restart();
    void factoryReset();

    PowerSnapshot getPowerSnapshot() const;

    MotionSnapshot getMotionSnapshot() const;
    void getMotionConfig(MotionConfig &cfg) const;
    bool setMotionConfig(const MotionConfig &cfg);
    void resetMotion();
    void stop();
    void brake();
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void rotateTo(float headingDegrees, uint8_t speed);
    void rotateBy(float degrees, uint8_t speed);

    WifiSnapshot getWifiSnapshot() const;
    const char *getHostname() const;
    void getWifiConfig(WifiConfig &cfg) const;
    bool setWifiConfig(const WifiConfig &cfg);
    void resetWifiConfig();
    bool setStationCredentials(const WifiCredentials &credentials);
    void resetStationCredentials();
    void getAccessPointCredentials(WifiCredentials &credentials) const;
    bool setAccessPointCredentials(const WifiCredentials &credentials);
    void resetAccessPointCredentials();
    void resetWifi();
};
#endif
