#include "IndicatorStorage.h"

void IndicatorStorage::begin() { _prefs.begin("indic_cfg", false); }

void IndicatorStorage::reset() { _prefs.clear(); }

void IndicatorStorage::loadConfig(IndicatorConfig &target) { 
    target.frequency = loadFrequency();
    target.resolution = loadResolution();
    target.intensity = loadIntensity();
}

void IndicatorStorage::saveConfig(const IndicatorConfig &cfg) { 
    saveFrequency(cfg.frequency);
    saveResolution(cfg.resolution);
    saveIntensity(cfg.intensity);
}

uint32_t IndicatorStorage::loadFrequency() {
    return _prefs.getULong("freq", IndicatorDefaults::frequency);
}

void IndicatorStorage::saveFrequency(uint32_t freq) { _prefs.putULong("freq", freq); }

uint8_t IndicatorStorage::loadResolution() {
    return _prefs.getUChar("res", IndicatorDefaults::resolution);
}

void IndicatorStorage::saveResolution(uint8_t res) { _prefs.putUChar("res", res); }

uint8_t IndicatorStorage::loadIntensity() {
    return _prefs.getUChar("intens", IndicatorDefaults::intensity);
}

void IndicatorStorage::saveIntensity(uint8_t intensity) { _prefs.putUChar("intens", intensity); }
