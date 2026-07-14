#ifndef MOTION_SNAPSHOT_H
#define MOTION_SNAPSHOT_H
#include "MotionState.h"

struct MotionSnapshot {
    MotionState state;
    bool stopped;
    struct MotionOutputSnapshot {
        int16_t left;
        int16_t right;
    } output;
    struct MotionHeadingSnapshot {
        float currentDegrees;
        float targetDegrees;
        float errorDegrees;
    } heading;
    struct MotionOdometrySnapshot {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
};
#endif
