#include "Robot.h"
#include "hardware/RobotHardwareConfig.h"
#include "platform/esp32/system/System.h"

Robot::Robot(PowerService &power, WifiController &wifi, MotionController &motion)
    : _power(power),
      _wifi(wifi),
      _motion(motion),
      _cfg{},
      _motionTimer{},
      _wifiTimer{} {}

bool Robot::begin(
    const RobotConfig &robotConfig,
    const MotionConfig &motionConfig,
    const WifiConfig &wifiConfig,
    const WifiCredentials &stationCredentials,
    const WifiCredentials &accessPointCredentials
) {
    _cfg = robotConfig;
    _motionTimer.stop();
    _wifiTimer.stop();
    bool ok = true;
    ok &= _wifi.begin(wifiConfig, stationCredentials, accessPointCredentials);
    _motion.begin(motionConfig, RobotHardwareConfig::MOTOR_PWM_FREQ, RobotHardwareConfig::ENCODER_SLOTS);
    ok &= _power.begin(RobotHardwareConfig::INA226_MAX_CURRENT_AMPS, RobotHardwareConfig::INA226_SHUNT_OHMS);
    return ok;
}

void Robot::update(uint32_t nowMs) {
    if (_motionTimer.poll(nowMs, _cfg.motionUpdateIntervalMs))
        _motion.update(nowMs);
    if (_wifiTimer.poll(nowMs, _cfg.wifiUpdateIntervalMs))
        _wifi.update(nowMs);
}

RobotSnapshot Robot::getSnapshot() {
    RobotSnapshot snapshot{};
    snapshot.system = Platform::System::getSnapshot();
    snapshot.power = _power.getSnapshot();
    snapshot.wifi = _wifi.getSnapshot();
    snapshot.motion = _motion.getSnapshot();
    return snapshot;
}

void Robot::getConfig(RobotConfig &config) const { config = _cfg; }

void Robot::setConfig(const RobotConfig &config) { _cfg = config; }

const char *Robot::getHostname() const { return _wifi.getHostname(); }

bool Robot::isTelemetryEnabled() const { return _cfg.telemetryEnabled; }
