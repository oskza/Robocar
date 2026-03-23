#ifndef ANALOG_JOYSTIC_CONTROLLER_H
#define ANALOG_JOYSTIC_CONTROLLER_H
#include <Timer.h>
#include <AnalogJoystic.h>
#include "storage/AnalogJoysticStorage.h"

class AnalogJoysticController {
private:
    AnalogJoystic &_joystic;
    AnalogJoysticStorage &_storage;
    Timer &_timer;
    bool _enabled;
public:
    AnalogJoysticController(AnalogJoystic &joystic, AnalogJoysticStorage &storage, Timer &timer);
    static int16_t mapToSignedPWM(uint16_t raw, uint16_t center, uint16_t deadzone);
    void init();
    bool tick(int16_t &vert, int16_t &horz);
    int16_t readVerticalSignedPWM() const;
    int16_t readHorizontalSignedPWM() const;
    void enable();
    void disable();
    void getConfig(AnalogJoysticConfig &target) const;
    void updateConfig(AnalogJoysticConfig &cfg);
    void resetConfig();
    void updateDeadzone(uint16_t deadzone);
    void updateIntervalMs(uint32_t intervalMs);
};
#endif
