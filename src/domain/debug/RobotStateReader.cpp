#include "RobotStateReader.h"

RobotStateReader::RobotStateReader(
        WifiManager &wifi,
        Ina226PowerMonitor &power,
        MotionController &motion,
        DifferentialDrive &differential,
        Odometry &odometry)
    : _wifi(wifi),
        _power(power),
        _motion(motion),
        _differential(differential),
        _odometry(odometry) {}

RobotSnapshot RobotStateReader::read(uint32_t uptimeMs) {
    RobotSnapshot snapshot;
    snapshot.uptimeMs = uptimeMs;
    snapshot.network.connected = _wifi.isConnected();
    snapshot.network.rssi = _wifi.getRssi();
    snapshot.network.localIp = _wifi.getLocalIp();
    snapshot.power.connected = _power.isConnected();
    snapshot.power.busVoltage = _power.getBusVoltage();
    snapshot.power.currentMilliamps = _power.getCurrentMa();
    snapshot.power.powerMilliwatts = _power.getPowerMw();
    snapshot.motion = _motion.getSnapshot();
    snapshot.drive.leftOutput = _differential.getLeftOutput();
    snapshot.drive.rightOutput = _differential.getRightOutput();
    snapshot.odometry.distanceMeters = _odometry.getMeters();
    snapshot.odometry.averageTicks = _odometry.getTicks();
    return snapshot;
}
