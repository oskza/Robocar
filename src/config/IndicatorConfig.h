#ifndef INDICATOR_CONFIG_H
#define INDICATOR_CONFIG_H
#include <stdint.h>

struct IndicatorConfig {
    uint32_t frequency;
    uint8_t resolution;
    uint8_t intensity;
};
#endif
