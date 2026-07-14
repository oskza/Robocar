#include "Robot.h"
#include "platform/esp32/system/System.h"
#include "app/system/FactoryReset.h"

Robot::Robot(
    PowerService &power,
    WifiController &wifi,
    MotionController &motion,
    WifiStorage &wifiStorage,
    MotionStorage &motionStorage,
    RobotStorage &storage
)
    : _power(power),
        _wifi(wifi),
        _motion(motion),
        _wifiStorage(wifiStorage),
        _motionStorage(motionStorage),
        _storage(storage),
        _cfg{},
        _lastMotionUpdateMs(0),
        _lastWifiUpdateMs(0) {}

bool Robot::_setTelemetryEnabled(bool enabled) {
    if (_cfg.telemetryEnabled == enabled)
        return true;
    _cfg.telemetryEnabled = enabled;
    const bool res = _storage.saveConfig(_cfg);
    if (!res)
        _cfg.telemetryEnabled = !enabled;
    return res;
}

void Robot::begin(
    uint32_t motorPwmFrequency,
    uint8_t encoderSlots,
    float powerMaxCurrentAmps,
    float powerShuntOhms
) {
    _storage.begin();
    _storage.loadConfig(_cfg);

    WifiConfig wifiCfg{};
    WifiCredentials station{};
    WifiCredentials accessPoint{};
    _wifiStorage.begin();
    _wifiStorage.loadConfig(wifiCfg);
    _wifiStorage.loadStationCredentials(station);
    _wifiStorage.loadAccessPointCredentials(accessPoint);
    _wifi.begin(wifiCfg, station, accessPoint);

    MotionConfig motionCfg{};
    _motionStorage.begin();
    _motionStorage.loadConfig(motionCfg);
    _motion.begin(motionCfg, motorPwmFrequency, encoderSlots);

    _power.begin(powerMaxCurrentAmps, powerShuntOhms);
}

void Robot::update() {
    const uint32_t now = getUptimeMs();
    if (now - _lastMotionUpdateMs >= _cfg.motionUpdateIntervalMs) {
        _lastMotionUpdateMs = now;
        _motion.update(now);
    }
    if (now - _lastWifiUpdateMs >= _cfg.wifiUpdateIntervalMs) {
        _lastWifiUpdateMs = now;
        _wifi.update(now);
    }
}

bool Robot::enableTelemetry() { return _setTelemetryEnabled(true); }

bool Robot::disableTelemetry() { return _setTelemetryEnabled(false); }

bool Robot::isTelemetryEnabled() { return _cfg.telemetryEnabled; }

RobotSnapshot Robot::getSnapshot() const {
    RobotSnapshot snapshot{};
    snapshot.system = Platform::System::getSnapshot();
    snapshot.network = _wifi.getSnapshot();
    snapshot.motion = _motion.getSnapshot();
    snapshot.power = _power.getSnapshot();
    return snapshot;
}

void Robot::getConfig(RobotConfig &cfg) const { cfg = _cfg; }

bool Robot::setConfig(const RobotConfig &cfg) {
    if (!_storage.saveConfig(cfg))
        return false;
    _cfg = cfg;
    return true;
}

void Robot::resetConfig() {
    _storage.resetConfig();
    _storage.loadConfig(_cfg);
}

SystemSnapshot Robot::getSystemSnapshot() const { Platform::System::getSnapshot(); }

uint32_t Robot::getUptimeMs() const { return Platform::System::getUptimeMs(); }

void Robot::restart() { Platform::System::restart(); }

void Robot::factoryReset() { Application::factoryReset(); }

PowerSnapshot Robot::getPowerSnapshot() const { return _power.getSnapshot(); }

MotionSnapshot Robot::getMotionSnapshot() const { return _motion.getSnapshot(); }

void Robot::getMotionConfig(MotionConfig &cfg) const { _motion.getConfig(cfg); }

bool Robot::setMotionConfig(const MotionConfig &cfg) {
    if (!_motionStorage.saveConfig(cfg))
        return false;
    _motion.setConfig(cfg);
    return true;
}

void Robot::resetMotion() {
    _motionStorage.resetConfig();
    MotionConfig cfg;
    _motionStorage.loadConfig(cfg);
    _motion.setConfig(cfg);
}

void Robot::stop() { _motion.stop(); }

void Robot::brake() { _motion.brake(); }

void Robot::drive(int16_t velocity, int16_t turn) { _motion.drive(velocity, turn); }

void Robot::driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) { _motion.driveFor(velocity, turn, durationMs); }

void Robot::driveDistance(int16_t velocity, float meters) { _motion.driveDistance(velocity, meters); }

void Robot::rotateTo(float headingDegrees, uint8_t speed) { _motion.rotateTo(headingDegrees, speed); }

void Robot::rotateBy(float degrees, uint8_t speed) { _motion.rotateBy(degrees, speed); }

WifiSnapshot Robot::getWifiSnapshot() const { return _wifi.getSnapshot(); }

const char* Robot::getHostname() const { return _wifi.getHostname(); }

void Robot::getWifiConfig(WifiConfig &cfg) const { _wifi.getConfig(cfg); }

bool Robot::setWifiConfig(const WifiConfig &cfg) {
    if (!_wifiStorage.saveConfig(cfg))
        return false;
    _wifi.setConfig(cfg);
    return true;
}

void Robot::resetWifiConfig() {
    _wifiStorage.resetConfig();
    WifiConfig cfg{};
    _wifiStorage.loadConfig(cfg);
    _wifi.setConfig(cfg);
}

bool Robot::setStationCredentials(const WifiCredentials &credentials) {
    if (!_wifiStorage.saveStationCredentials(credentials))
        return false;
    _wifi.setStationCredentials(credentials);
    return true;
}

void Robot::resetStationCredentials() {
    _wifiStorage.resetStationCredentials();
    WifiCredentials credentials{};
    _wifiStorage.loadStationCredentials(credentials);
    _wifi.setStationCredentials(credentials);
}

void Robot::getAccessPointCredentials(WifiCredentials &credentials) const { _wifi.getAccessPointCredentials(credentials); }

bool Robot::setAccessPointCredentials(const WifiCredentials &credentials) {
    if (!_wifiStorage.saveAccessPointCredentials(credentials))
        return false;
    _wifi.setAccessPointCredentials(credentials);
    return true;
}

void Robot::resetAccessPointCredentials() {
    _wifiStorage.resetAccessPointCredentials();
    WifiCredentials credentials{};
    _wifiStorage.loadAccessPointCredentials(credentials);
    _wifi.setAccessPointCredentials(credentials);
}

void Robot::resetWifi() {
    _wifiStorage.resetAll();

    WifiConfig cfg{};
    WifiCredentials station{};
    WifiCredentials accessPoint{};

    _wifiStorage.loadConfig(cfg);
    _wifiStorage.loadStationCredentials(station);
    _wifiStorage.loadAccessPointCredentials(accessPoint);

    _wifi.begin(cfg, station, accessPoint);
}
