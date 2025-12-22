#include "AnalogJoysticController.h"

AnalogJoysticController::AnalogJoysticController(AnalogJoystic *joystic, Timer *timer, uint16_t deadzone) 
                                                    : _joystic(joystic), _timer(timer), _deadzone(deadzone) {}

int16_t AnalogJoysticController::_toSignedPWM(uint16_t raw, uint16_t center) const {
    return (raw < center) 
            ? map(raw, center, 0, 0, 255)
            : map(raw, center, 4095, 0, -255);
}

void AnalogJoysticController::init(uint32_t msInterval) {
    _joystic->calibrateCenter();
    _timer->setTimeout(msInterval);
    _timer->start();
}

bool AnalogJoysticController::tick(int16_t &vert, int16_t &horz) {
    if(!_timer->tick())
        return false;
    vert = readVerticalPWM();
    horz = readHorizontalPWM();
    _timer->refresh();
    return true;
}

uint16_t AnalogJoysticController::readVerticalRaw() const { return _joystic->readVertical(); }

uint16_t AnalogJoysticController::readHorizontalRaw() const { return _joystic->readHorizontal(); }

int16_t AnalogJoysticController::readVerticalPWM() const { 
    return _toSignedPWM(_joystic->readVertical(), _joystic->getVerticalCenter());
}

int16_t AnalogJoysticController::readHorizontalPWM() const {
    return _toSignedPWM(_joystic->readHorizontal(), _joystic->getHorizontalCenter());
}

bool AnalogJoysticController::isInDeadzone(int16_t vert, int16_t horz) const {
    return abs(vert) <= _deadzone && abs(horz) <= _deadzone;
}

void AnalogJoysticController::setDeadzone(uint16_t deadzone) { _deadzone = deadzone; }
