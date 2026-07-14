#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include "domain/system/SystemSnapshot.h"
#include "subsystems/power/PowerSnapshot.h"
#include "subsystems/network/WifiSnapshot.h"
#include "subsystems/motion/MotionSnapshot.h"

struct RobotSnapshot {
    SystemSnapshot system;
    PowerSnapshot power;
    WifiSnapshot network;
    MotionSnapshot motion;
};
#endif
