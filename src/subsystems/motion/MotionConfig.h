#ifndef MOTION_CONFIG_H
#define MOTION_CONFIG_H
#include <stdint.h>

struct MotionConfig {
    float wheelDiameterMeters;
    float wheelCircumferenceFactor;
    uint8_t wheelAcceleration;
    uint8_t motorLeftMinPwm;
    uint8_t motorRightMinPwm;
    float headingToleranceDegrees;
};
#endif
