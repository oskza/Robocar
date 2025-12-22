#ifndef ANALOG_JOYSTIC_CONTROLLER_H
#define ANALOG_JOYSTIC_CONTROLLER_H
#include <AnalogJoystic.h>
#include <Timer.h>

class AnalogJoysticController {
private:
    AnalogJoystic *_joystic;
    Timer *_timer;
    uint16_t _deadzone;
    int16_t _toSignedPWM(uint16_t raw, uint16_t center) const;
public:
    AnalogJoysticController(AnalogJoystic *joystic, Timer *timer, uint16_t deadzone);
    void init(uint32_t msInterval);
    bool tick(int16_t &vert, int16_t &horz);
    uint16_t readVerticalRaw() const;
    uint16_t readHorizontalRaw() const;
    int16_t readVerticalPWM() const;
    int16_t readHorizontalPWM() const;
    bool isInDeadzone(int16_t vert, int16_t horz) const;
    void setDeadzone(uint16_t deadzone);
};
#endif
