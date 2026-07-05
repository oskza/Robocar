#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include <stdint.h>
#include <IPAddress.h>
#include "system/SystemSnapshot.h"
#include "network/WifiSnapshot.h"
#include "motion/MotionSnapshot.h"

struct RobotSnapshot {
    SystemSnapshot system;
    WifiSnapshot network;
    MotionSnapshot motion;
    struct {
        bool connected;
        float busVoltage;
        float currentMilliamps;
        float powerMilliwatts;
    } power;
    struct {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
};
#endif
