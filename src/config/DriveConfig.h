#ifndef DRIVE_CONFIG_H
#define DRIVE_CONFIG_H
#include <stdint.h>

struct DriveConfig {
    uint32_t frequency;
    uint8_t resolution;
    double wheelDiameter;
    double circCorrection;
    uint8_t encoderSlots;
    uint8_t motorRightMinPWM;
    uint8_t motorLeftMinPWM;
};
#endif
