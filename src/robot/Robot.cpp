#include "Robot.h"
#include "hardware/RobotHardwareConfig.h"
#include "platform/esp32/system/System.h"

Robot::Robot(PowerService &power, WifiController &wifi, MotionController &motion)
    : _power(power),
      _wifi(wifi),
      _motion(motion),
      _config{},
      _lastMotionUpdateMs(0),
      _lastWifiUpdateMs(0) {}

bool Robot::begin(
    const RobotConfig &robotConfig,
    const MotionConfig &motionConfig,
    const WifiConfig &wifiConfig,
    const WifiCredentials &stationCredentials,
    const WifiCredentials &accessPointCredentials
) {
    _config = robotConfig;
    _lastMotionUpdateMs = 0;
    _lastWifiUpdateMs = 0;
    bool ok = true;
    ok &= _wifi.begin(wifiConfig, stationCredentials, accessPointCredentials);
    _motion.begin(motionConfig, RobotHardwareConfig::MOTOR_PWM_FREQ, RobotHardwareConfig::ENCODER_SLOTS);
    ok &= _power.begin(RobotHardwareConfig::INA226_MAX_CURRENT_AMPS, RobotHardwareConfig::INA226_SHUNT_OHMS);
    return ok;
}

void Robot::update(uint32_t nowMs) {
    if (nowMs - _lastMotionUpdateMs >= _config.motionUpdateIntervalMs) {
        _lastMotionUpdateMs = nowMs;
        _motion.update(nowMs);
    }
    if (nowMs - _lastWifiUpdateMs >= _config.wifiUpdateIntervalMs) {
        _lastWifiUpdateMs = nowMs;
        _wifi.update(nowMs);
    }
}

RobotSnapshot Robot::getSnapshot() {
    RobotSnapshot snapshot{};
    snapshot.system = Platform::System::getSnapshot();
    snapshot.power = _power.getSnapshot();
    snapshot.wifi = _wifi.getSnapshot();
    snapshot.motion = _motion.getSnapshot();
    return snapshot;
}

void Robot::getConfig(RobotConfig &config) const { config = _config; }

void Robot::setConfig(const RobotConfig &config) { _config = config; }

bool Robot::isTelemetryEnabled() const { return _config.telemetryEnabled; }
