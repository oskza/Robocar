#ifndef MOTION_SNAPSHOT_H
#define MOTION_SNAPSHOT_H
#include "MotionState.h"

struct MotionSnapshot {
    MotionState state;
    bool stopped;
    float currentHeadingDegrees;
    float targetHeadingDegrees;
    float headingErrorDegrees;
};
#endif
