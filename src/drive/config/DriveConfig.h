#ifndef DRIVE_CONFIG_H
#define DRIVE_CONFIG_H
#include <stdint.h>

struct DriveConfig {
    uint32_t pwmFrequency = 1000;
    uint8_t pwmResolution = 8;
    float wheelDiameter = 0.067f;
    float wheelCircumferenceFactor = 1.00f;
    uint8_t encoderSlots = 20;
    uint8_t motorRightMinPwm = 110;
    uint8_t motorLeftMinPwm = 110;
};
#endif
