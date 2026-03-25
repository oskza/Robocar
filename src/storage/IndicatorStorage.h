#ifndef INDICATOR_STORAGE_H
#define INDICATOR_STORAGE_H
#include <Preferences.h>
#include "config/IndicatorConfig.h"
#include "defaults/IndicatorDefaults.h"

class IndicatorStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(IndicatorConfig &target);
    void saveConfig(const IndicatorConfig &cfg);
    uint32_t loadFrequency();
    void saveFrequency(uint32_t freq);
    uint8_t loadResolution();
    void saveResolution(uint8_t res);
    uint8_t loadIntensity();
    void saveIntensity(uint8_t intensity);
};
#endif
