#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H
#include <stdint.h>

struct DeviceConfig {
    uint32_t statusReportIntervalMs;
    uint8_t indicatorIntensity;
};
#endif
