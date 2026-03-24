#ifndef DEFAULT_CONFIG_H
#define DEFAULT_CONFIG_H
#include <stdint.h>

struct DeviceDefaults {
    static constexpr uint32_t statusReportIntervalMs = 5000;
    static constexpr uint8_t indicatorIntensity = 180;
};
#endif
