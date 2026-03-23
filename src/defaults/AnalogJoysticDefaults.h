#ifndef ANALOG_JOYSTIC_DEFAULTS_H
#define ANALOG_JOYSTIC_DEFAULTS_H
#include <stdint.h>

struct AnalogJoysticDefaults {
    static constexpr uint16_t deadzone = 112;
    static constexpr uint32_t intervalMs = 20;
};
#endif
