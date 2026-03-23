#include "AnalogJoysticStorage.h"

void AnalogJoysticStorage::begin() { _prefs.begin("joystic_cfg", false); }

void AnalogJoysticStorage::reset() { _prefs.clear(); }

void AnalogJoysticStorage::loadConfig(AnalogJoysticConfig &target) {
    target.deadzone =  _prefs.getUShort("deadzone", AnalogJoysticDefaults::deadzone);
    target.intervalMs = _prefs.getULong("interval", AnalogJoysticDefaults::intervalMs);
}

void AnalogJoysticStorage::saveConfig(const AnalogJoysticConfig &cfg) {
    saveDeadzone(cfg.deadzone);
    saveIntervalMs(cfg.intervalMs);
}

void AnalogJoysticStorage::saveDeadzone(uint16_t deadzone) { _prefs.putUShort("deadzone", deadzone); }

void AnalogJoysticStorage::saveIntervalMs(uint32_t ms) { _prefs.putULong("interval", ms); }
