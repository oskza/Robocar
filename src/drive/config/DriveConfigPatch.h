#ifndef DRIVE_CONFIG_PATCH_H
#define DRIVE_CONFIG_PATCH_H
#include <stdint.h>

struct DriveConfigPatch {
    uint32_t pwmFrequency;
    uint8_t pwmResolution;
    float wheelDiameter;
    float wheelCircumferenceFactor;
    uint8_t encoderSlots;
    uint8_t motorRightMinPwm;
    uint8_t motorLeftMinPwm;
    bool hasFreq;
    bool hasRes;
    bool hasWheelDiam;
    bool hasCircumFactor;
    bool hasEncSlots;
    bool hasRightMinPWM;
    bool hasLeftMinPWM;
};
#endif
