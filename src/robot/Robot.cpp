#include "Robot.h"

Robot::Robot(
    SystemController &system,
    PowerController &power,
    WifiController &wifi,
    WifiStorage &wifiStorage,
    MotionController &motion,
    MotionStorage &motionStorage,
    RobotStorage &storage
)
    : _system(system),
        _power(power),
        _wifi(wifi),
        _wifiStorage(wifiStorage),
        _motion(motion),
        _motionStorage(motionStorage),
        _storage(storage),
        _cfg{},
        _lastMotionUpdateMs(0),
        _lastWifiUpdateMs(0) {}

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
    const uint32_t now = _system.getUptimeMs();
    if (now - _lastMotionUpdateMs >= _cfg.motionUpdateIntervalMs) {
        _lastMotionUpdateMs = now;
        _motion.update(now);
    }
    if (now - _lastWifiUpdateMs >= _cfg.wifiUpdateIntervalMs) {
        _lastWifiUpdateMs = now;
        _wifi.update(now);
    }
}

RobotSnapshot Robot::getSnapshot() const {
    RobotSnapshot snapshot{};
    snapshot.system = _system.getSnapshot();
    snapshot.network = _wifi.getSnapshot();
    snapshot.motion = _motion.getSnapshot();
    snapshot.power = _power.getSnapshot();
    return snapshot;
}
