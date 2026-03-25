#ifndef INDICATOR_DEFAULTS_H
#define INDICATOR_DEFAULTS_H
#include <stdint.h>

struct IndicatorDefaults {
    static constexpr uint32_t frequency = 5000;
    static constexpr uint8_t resolution = 8;
    static constexpr uint8_t intensity = 180;
};
#endif
