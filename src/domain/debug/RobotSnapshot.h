#ifndef ROBOT_SNAPSHOT_H
#define ROBOT_SNAPSHOT_H
#include <stdint.h>
#include <IPAddress.h>
#include "../motion/MotionSnapshot.h"

struct RobotSnapshot {
    uint32_t uptimeMs;
    struct {
        bool connected;
        int32_t rssi;
        IPAddress localIp;
    } network;
    MotionSnapshot motion;
    struct {
        int16_t leftOutput;
        int16_t rightOutput;
    } drive;
    struct {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
};
#endif
