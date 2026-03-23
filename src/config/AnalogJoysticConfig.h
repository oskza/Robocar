#ifndef ANALOG_JOYSTIC_CONFIG_H
#define ANALOG_JOYSTIC_CONFIG_H
#include <stdint.h>

struct AnalogJoysticConfig {
    uint16_t deadzone;
    uint32_t intervalMs;
};
#endif
