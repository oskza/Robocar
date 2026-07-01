#ifndef ROBOT_STATE_READER_H
#define ROBOT_STATE_READER_H
#include <WifiManager.h>
#include <Ina226PowerMonitor.h>
#include "../drive/DifferentialDrive.h"
#include "../motion/MotionController.h"
#include "../odometry/Odometry.h"
#include "RobotSnapshot.h"

class RobotStateReader {
private:
    WifiManager &_wifi;
    Ina226PowerMonitor &_power;
    MotionController &_motion;
    DifferentialDrive &_differential;
    Odometry &_odometry;
public:
    RobotStateReader(WifiManager &wifi, Ina226PowerMonitor &power, MotionController &motion, DifferentialDrive &differential, Odometry &odometry);
    RobotSnapshot read(uint32_t uptimeMs);
};
#endif
