#include "IndicatorController.h"

IndicatorController::IndicatorController(RGBLED &led, IndicatorStorage &storage)
                                            : _led(led), _storage(storage), 
                                                _intensity(IndicatorDefaults::intensity) {}

void IndicatorController::init() {
    IndicatorConfig config;
    _storage.begin();
    _storage.loadConfig(config);
    _led.init(config.frequency, config.resolution);
    _intensity = config.intensity;
}

void IndicatorController::off() { _led.off(); }

void IndicatorController::error() { _led.setColor(_intensity, 0, 0); }

void IndicatorController::success() { _led.setColor(0, _intensity, 0); }

void IndicatorController::waiting() { _led.setColor(0, 0, _intensity); }

void IndicatorController::resetConfig() {
    _storage.reset(); 
    updateFrequency(IndicatorDefaults::frequency);
    updateResolution(IndicatorDefaults::resolution);
    updateIntensity(IndicatorDefaults::intensity);
}

void IndicatorController::getConfig(IndicatorConfig &target) const { _storage.loadConfig(target); }

void IndicatorController::updateConfig(IndicatorConfig &cfg) {
    updateFrequency(IndicatorDefaults::frequency);
    updateResolution(IndicatorDefaults::resolution);
    updateIntensity(IndicatorDefaults::intensity);
}

/** TODO: apply */
void IndicatorController::updateFrequency(uint32_t freq) { _storage.saveFrequency(freq); }

/** TODO: apply */
void IndicatorController::updateResolution(uint8_t res) { _storage.saveResolution(res); }

void IndicatorController::updateIntensity(uint8_t intensity) {
    if (_intensity == intensity)
        return;
    _intensity = intensity;
    _storage.saveIntensity(intensity);
}
