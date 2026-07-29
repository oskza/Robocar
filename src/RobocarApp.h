#ifndef ROBOCAR_APP_H
#define ROBOCAR_APP_H
#include <Timer.h>
#include "motion/MotionController.h"
#include "motion/MotionStorage.h"
#include "network/WifiController.h"
#include "network/WifiStorage.h"
#include "ota/OtaService.h"
#include "robot/Robot.h"
#include "robot/RobotStorage.h"
#include "telemetry/TelemetryService.h"
#include "websocket/WebSocketService.h"

class RobocarApp {
private:
    Robot &_robot;
    MotionController &_motion;
    WifiController &_wifi;
    RobotStorage &_robotStorage;
    MotionStorage &_motionStorage;
    WifiStorage &_wifiStorage;
    OtaService &_ota;
    WebSocketService &_webSocket;
    TelemetryService &_telemetry;
    Timer _motionTimer;
    Timer _wifiTimer;
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
        MotionController &motion,
        WifiController &wifi,
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
