#include "RobocarApp.h"

RobocarApp::RobocarApp(
    Robot &robot,
    MotionController &motion,
    WifiController &wifi,
    RobotStorage &robotStorage,
    MotionStorage &motionStorage,
    WifiStorage &wifiStorage,
    OtaService &ota,
    WebSocketService &webSocket,
    TelemetryService &telemetry
)
    : _robot(robot),
      _motion(motion),
      _wifi(wifi),
      _robotStorage(robotStorage),
      _motionStorage(motionStorage),
      _wifiStorage(wifiStorage),
      _ota(ota),
      _webSocket(webSocket),
      _telemetry(telemetry),
      _motionTimer{},
      _wifiTimer{},
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
    _motionTimer.stop();
    _wifiTimer.stop();
    _started = true;
    return ok;
}

void RobocarApp::update(uint32_t nowMs) {
    if (!_started)
        return;
    RobotConfig cfg{};
    _robot.getConfig(cfg);
    if (_motionTimer.poll(nowMs, cfg.motionUpdateIntervalMs))
        _motion.update(nowMs);
    if (_wifiTimer.poll(nowMs, cfg.wifiUpdateIntervalMs))
        _wifi.update(nowMs);
    _ota.update();
    _webSocket.update();
    if (cfg.telemetryEnabled)
        _telemetry.update(nowMs);
}

bool RobocarApp::isStarted() const { return _started; }
