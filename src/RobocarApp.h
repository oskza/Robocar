#ifndef ROBOCAR_APP_H
#define ROBOCAR_APP_H
#include "motion/MotionStorage.h"
#include "network/WifiStorage.h"
#include "ota/OtaService.h"
#include "robot/Robot.h"
#include "robot/RobotStorage.h"
#include "telemetry/TelemetryService.h"
#include "websocket/WebSocketService.h"

class RobocarApp {
private:
    Robot &_robot;
    RobotStorage &_robotStorage;
    MotionStorage &_motionStorage;
    WifiStorage &_wifiStorage;
    OtaService &_ota;
    WebSocketService &_webSocket;
    TelemetryService &_telemetry;
    bool _started;
    bool _loadConfiguration(
        RobotConfig &robotConfig,
        MotionConfig &motionConfig,
        WifiConfig &wifiConfig,
        WifiCredentials &stationCredentials,
        WifiCredentials &accessPointCredentials
    );
public:
    RobocarApp(
        Robot &robot,
        RobotStorage &robotStorage,
        MotionStorage &motionStorage,
        WifiStorage &wifiStorage,
        OtaService &ota,
        WebSocketService &webSocket,
        TelemetryService &telemetry
    );
    bool begin();
    void update(uint32_t nowMs);
    bool isStarted() const;
};
#endif
