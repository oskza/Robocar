#ifndef MOTION_SNAPSHOT_H
#define MOTION_SNAPSHOT_H
#include "MotionState.h"

struct MotionSnapshot {
    MotionState state = MotionState::IDLE;
    bool stopped = true;
    struct MotionOutputSnapshot {
        int16_t left = 0;
        int16_t right = 0;
    } output;
    struct MotionHeadingSnapshot {
        bool available = false;
        float currentDegrees = 0.0f;
        float targetDegrees = 0.0f;
        float errorDegrees = 0.0f;
    } heading;
    struct MotionOdometrySnapshot {
        float distanceMeters = 0.0f;
        uint32_t averageTicks = 0;
    } odometry;
};
#endif
