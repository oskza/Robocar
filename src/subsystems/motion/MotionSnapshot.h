#ifndef MOTION_SNAPSHOT_H
#define MOTION_SNAPSHOT_H
#include "MotionState.h"

struct MotionSnapshot {
    MotionState state;
    bool stopped;
    struct {
        int16_t left;
        int16_t right;
    } output;
    struct {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
    struct {
        float currentDegrees;
        float targetDegrees;
        float errorDegrees;
    } heading;
};
#endif
