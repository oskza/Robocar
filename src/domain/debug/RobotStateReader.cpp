#include "RobotStateReader.h"

RobotStateReader::RobotStateReader(
        WifiManager &wifi,
        MotionController &motion,
        DifferentialDrive &differential,
        Odometry &odometry)
    : _wifi(wifi),
      _motion(motion),
      _differential(differential),
      _odometry(odometry) {}

RobotSnapshot RobotStateReader::read(uint32_t uptimeMs) const {
    RobotSnapshot snapshot;
    snapshot.uptimeMs = uptimeMs;
    snapshot.network.connected = _wifi.isConnected();
    snapshot.network.rssi = _wifi.getRssi();
    snapshot.network.localIp = _wifi.getLocalIp();
    snapshot.motion = _motion.getSnapshot();
    snapshot.drive.leftOutput = _differential.getLeftOutput();
    snapshot.drive.rightOutput = _differential.getRightOutput();
    snapshot.odometry.distanceMeters = _odometry.getMeters();
    snapshot.odometry.averageTicks = _odometry.getTicks();
    return snapshot;
}
