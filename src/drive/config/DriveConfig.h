#ifndef DRIVE_CONFIG_H
#define DRIVE_CONFIG_H
#include <stdint.h>

struct DriveConfig {
    uint32_t pwmFrequency = 1000;
    uint8_t pwmResolution = 8;
    float wheelDiameter = 0.067f;
    float wheelCircumferenceFactor = 1.00f;
    uint8_t encoderSlots = 20;
    uint8_t motorRightMinPWM = 110;
    uint8_t motorLeftMinPWM = 110;
};
#endif
