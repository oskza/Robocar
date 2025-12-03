#include "AnalogJoystic.h"

AnalogJoystic::AnalogJoystic(uint8_t vertPin, uint8_t horzPin, uint16_t deadzone) 
        : _vertPin(vertPin), _horzPin(horzPin), _deadzone(deadzone), 
            _vertCenter(2048), _horzCenter(2048) {}

void AnalogJoystic::calibrateCenter() {
    calibrateVerticalCenter();
    calibrateHorizontalCenter();
}

void AnalogJoystic::calibrateVerticalCenter() { _vertCenter = analogRead(_vertPin); }

void AnalogJoystic::calibrateHorizontalCenter() { _horzCenter = analogRead(_horzPin); }

uint16_t AnalogJoystic::readVertical() const { return analogRead(_vertPin); }

uint16_t AnalogJoystic::getVerticalCenter() const { return _vertCenter; }

uint16_t AnalogJoystic::getVerticalDeadzoneMin() const { return _vertCenter - _deadzone; }

uint16_t AnalogJoystic::getVerticalDeadzoneMax() const { return _vertCenter + _deadzone; }

uint16_t AnalogJoystic::readHorizontal() const { return analogRead(_horzPin); }

uint16_t AnalogJoystic::getHorizontalCenter() const { return _horzCenter; }

uint16_t AnalogJoystic::getHorizontalDeadzoneMin() const { return _horzCenter - _deadzone; }

uint16_t AnalogJoystic::getHorizontalDeadzoneMax() const { return _horzCenter + _deadzone; }

bool AnalogJoystic::isInDeadzone(uint16_t vert, uint16_t horz) const {
    return isVerticalInDeadzone(vert) && isHorizontalInDeadzone(horz);
}

bool AnalogJoystic::isVerticalInDeadzone(uint16_t val) const { 
    return !isVerticalBelowDeadzone(val) && !isVerticalAboveDeadzone(val);
}

bool AnalogJoystic::isVerticalBelowDeadzone(uint16_t val) const { return val < getVerticalDeadzoneMin(); }

bool AnalogJoystic::isVerticalAboveDeadzone(uint16_t val) const { return val > getVerticalDeadzoneMax(); }

bool AnalogJoystic::isHorizontalInDeadzone(uint16_t val) const {
    return !isHorizontalBelowDeadzone(val) && !isHorizontalAboveDeadzone(val);
}

bool AnalogJoystic::isHorizontalBelowDeadzone(uint16_t val) const { return val < getHorizontalDeadzoneMin(); }

bool AnalogJoystic::isHorizontalAboveDeadzone(uint16_t val) const { return val > getHorizontalDeadzoneMax(); }
