#ifndef MOTION_SNAPSHOT_H
#define MOTION_SNAPSHOT_H
#include <stdint.h>
#include "MotionState.h"

struct MotionSnapshot {
    MotionState state;
    bool stopped;
    float currentHeadingDegrees;
    float targetHeadingDegrees;
    float headingErrorDegrees;
    int16_t leftOutput;
    int16_t rightOutput;
    struct {
        float distanceMeters;
        uint32_t averageTicks;
    } odometry;
};
#endif
