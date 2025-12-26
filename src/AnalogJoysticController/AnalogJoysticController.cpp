#include "AnalogJoysticController.h"

AnalogJoysticController::AnalogJoysticController(AnalogJoystic &joystic, Timer &timer, uint16_t deadzone) 
                                                    : _joystic(joystic), _timer(timer), _deadzone(deadzone) {}

int16_t AnalogJoysticController::_mapToSignedPWM(uint16_t raw, uint16_t center) const {
    if (raw < center - _deadzone)
        return map(raw, center - _deadzone, ANALOG_JOYSTIC_MIN_VALUE, 0, 255);
    if (raw > center + _deadzone)
        return map(raw, center + _deadzone, ANALOG_JOYSTIC_MAX_VALUE, 0, -255);
    return 0;
}

void AnalogJoysticController::init(uint32_t msInterval) {
    _joystic.calibrateCenter();
    _timer.setTimeout(msInterval);
    _timer.start();
}

bool AnalogJoysticController::tick(int16_t &vert, int16_t &horz) {
    if(!_timer.tick())
        return false;
    vert = readVerticalSignedPWM();
    horz = readHorizontalSignedPWM();
    _timer.refresh();
    return true;
}

int16_t AnalogJoysticController::readVerticalSignedPWM() const { 
    return _mapToSignedPWM(_joystic.readVertical(), _joystic.getVerticalCenter());
}

int16_t AnalogJoysticController::readHorizontalSignedPWM() const {
    return _mapToSignedPWM(_joystic.readHorizontal(), _joystic.getHorizontalCenter());
}

void AnalogJoysticController::setDeadzone(uint16_t deadzone) { _deadzone = deadzone; }
