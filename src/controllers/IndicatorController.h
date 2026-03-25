#ifndef INDICATOR_CONTROLLER_H
#define INDICATOR_CONTROLLER_H
#include <RGBLED.h>
#include "storage/IndicatorStorage.h"

class IndicatorController {
private:
    RGBLED &_led;
    IndicatorStorage &_storage;
    uint8_t _intensity;
public:
    IndicatorController(RGBLED &led, IndicatorStorage &storage);
    void init();
    void off();
    void error();
    void success();
    void waiting();
    void resetConfig();
    void getConfig(IndicatorConfig &target) const;
    void updateConfig(IndicatorConfig &cfg);
    void updateFrequency(uint32_t freq);
    void updateResolution(uint8_t res);
    void updateIntensity(uint8_t intensity);
};
#endif
