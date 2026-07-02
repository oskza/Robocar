#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include <stdint.h>
#include <IPAddress.h>
#include "motion/MotionSnapshot.h"

struct RobotSnapshot {
    uint32_t uptimeMs;
    struct {
        bool connected;
        int32_t rssi;
        IPAddress localIp;
    } network;
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
