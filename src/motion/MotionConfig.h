#ifndef MOTION_CONFIG_H
#define MOTION_CONFIG_H
#include <stdint.h>

struct MotionConfig {
    float wheelDiameterMeters = 0.0f;
    float wheelCircumferenceFactor = 1.0f;
    uint8_t wheelAcceleration = 0;
    uint8_t motorLeftMinPwm = 0;
    uint8_t motorRightMinPwm = 0;
    float headingToleranceDegrees = 0.0f;
};
#endif
