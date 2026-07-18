#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include "motion/MotionSnapshot.h"
#include "network/WifiSnapshot.h"
#include "power/PowerSnapshot.h"
#include "system/SystemSnapshot.h"

struct RobotSnapshot {
    SystemSnapshot system{};
    PowerSnapshot power{};
    WifiSnapshot wifi{};
    MotionSnapshot motion{};
};
#endif
