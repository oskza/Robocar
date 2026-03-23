#ifndef ANALOG_JOYSTIC_STORAGE_H
#define ANALOG_JOYSTIC_STORAGE_H
#include <Preferences.h>
#include "config/AnalogJoysticConfig.h"
#include "defaults/AnalogJoysticDefaults.h"

class AnalogJoysticStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(AnalogJoysticConfig &target);
    void saveConfig(const AnalogJoysticConfig &cfg);
    void saveDeadzone(uint16_t deadzone);
    void saveIntervalMs(uint32_t ms);
};
#endif
