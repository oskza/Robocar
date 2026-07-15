#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include "system/SystemSnapshot.h"
#include "power/PowerSnapshot.h"
#include "network/WifiSnapshot.h"
#include "motion/MotionSnapshot.h"

struct RobotSnapshot {
    SystemSnapshot system;
    PowerSnapshot power;
    WifiSnapshot network;
    MotionSnapshot motion;
};
#endif
