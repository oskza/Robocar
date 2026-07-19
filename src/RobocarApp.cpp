#include "RobocarApp.h"

RobocarApp::RobocarApp(
    Robot &robot,
    RobotStorage &robotStorage,
    MotionStorage &motionStorage,
    WifiStorage &wifiStorage,
    OtaService &ota,
    WebSocketService &webSocket,
    TelemetryService &telemetry
)
    : _robot(robot),
      _robotStorage(robotStorage),
      _motionStorage(motionStorage),
      _wifiStorage(wifiStorage),
      _ota(ota),
      _webSocket(webSocket),
      _telemetry(telemetry),
      _started(false) {}

bool RobocarApp::_loadConfiguration(
    RobotConfig &robotConfig,
    MotionConfig &motionConfig,
    WifiConfig &wifiConfig,
    WifiCredentials &stationCredentials,
    WifiCredentials &accessPointCredentials
) {
    bool ok = true;
    ok &= _robotStorage.loadConfig(robotConfig);
    ok &= _motionStorage.loadConfig(motionConfig);
    ok &= _wifiStorage.loadConfig(wifiConfig);
    ok &= _wifiStorage.loadStationCredentials(stationCredentials);
    ok &= _wifiStorage.loadAccessPointCredentials(accessPointCredentials);
    return ok;
}

bool RobocarApp::begin() {
    if (_started)
        return true;
    RobotConfig robotConfig{};
    MotionConfig motionConfig{};
    WifiConfig wifiConfig{};
    WifiCredentials stationCredentials{};
    WifiCredentials accessPointCredentials{};
    bool ok = true;
    ok &= _loadConfiguration(
        robotConfig,
        motionConfig,
        wifiConfig,
        stationCredentials,
        accessPointCredentials
    );
    ok &= _robot.begin(
        robotConfig,
        motionConfig,
        wifiConfig,
        stationCredentials,
        accessPointCredentials
    );
    ok &= _ota.begin(_robot.getHostname());
    ok &= _webSocket.begin();
    _started = true;
    return ok;
}

void RobocarApp::update(uint32_t nowMs) {
    if (!_started)
        return;
    _robot.update(nowMs);
    _ota.update();
    _webSocket.update();
    if (_robot.isTelemetryEnabled())
        _telemetry.update(nowMs);
}

bool RobocarApp::isStarted() const { return _started; }
