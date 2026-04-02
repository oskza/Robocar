#ifndef DRIVE_DEFAULTS_H
#define DRIVE_DEFAULTS_H
#include <stdint.h>

struct DriveDefaults {
    static constexpr uint32_t frequency = 1000;
    static constexpr uint8_t resolution = 8;
    static constexpr double wheelDiameter = 0.067f;
    static constexpr double circCorrection = 1.00f;
    static constexpr uint8_t encoderSlots = 20;
    static constexpr uint8_t motorRightMinPWM = 110;
    static constexpr uint8_t motorLeftMinPWM = 110;
};
#endif
