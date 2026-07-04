#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include <stdint.h>
#include <IPAddress.h>
#include "network/WifiSnapshot.h"
#include "motion/MotionSnapshot.h"

struct RobotSnapshot {
    uint32_t uptimeMs;
    WifiSnapshot network;
    struct {
        bool connected;
        float busVoltage;
        float currentMilliamps;
        float powerMilliwatts;
    } power;
    MotionSnapshot motion;
    struct {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
};
#endif
