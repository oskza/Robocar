#include "Robot.h"
#include "hardware/RobotHardwareConfig.h"
#include "platform/esp32/system/System.h"

Robot::Robot(PowerService &power, WifiController &wifi, MotionController &motion)
    : _power(power),
      _wifi(wifi),
      _motion(motion),
      _cfg{} {}

bool Robot::begin(
    const RobotConfig &robotConfig,
    const MotionConfig &motionConfig,
    const WifiConfig &wifiConfig,
    const WifiCredentials &stationCredentials,
    const WifiCredentials &accessPointCredentials
) {
    _cfg = robotConfig;
    bool ok = true;
    ok &= _wifi.begin(wifiConfig, stationCredentials, accessPointCredentials);
    _motion.begin(motionConfig, RobotHardwareConfig::MOTOR_PWM_FREQ, RobotHardwareConfig::ENCODER_SLOTS);
    ok &= _power.begin(RobotHardwareConfig::INA226_MAX_CURRENT_AMPS, RobotHardwareConfig::INA226_SHUNT_OHMS);
    return ok;
}

RobotSnapshot Robot::getSnapshot() const {
    RobotSnapshot snapshot{};
    snapshot.system = Platform::System::getSnapshot();
    snapshot.power = _power.getSnapshot();
    snapshot.wifi = _wifi.getSnapshot();
    snapshot.motion = _motion.getSnapshot();
    return snapshot;
}

void Robot::getConfig(RobotConfig &cfg) const { cfg = _cfg; }

void Robot::setConfig(const RobotConfig &cfg) { _cfg = cfg; }

const char *Robot::getHostname() const { return _wifi.getHostname(); }

bool Robot::isTelemetryEnabled() const { return _cfg.telemetryEnabled; }
