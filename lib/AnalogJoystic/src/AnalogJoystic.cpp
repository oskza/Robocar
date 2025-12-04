#include "AnalogJoystic.h"

AnalogJoystic::AnalogJoystic(uint8_t vertPin, uint8_t horzPin, uint16_t deadzone) 
        : _vertPin(vertPin), _horzPin(horzPin), _deadzone(deadzone), 
            _vertCenter(2048), _horzCenter(2048) {}

void AnalogJoystic::calibrateVerticalCenter() { _vertCenter = analogRead(_vertPin); }
void AnalogJoystic::calibrateHorizontalCenter() { _horzCenter = analogRead(_horzPin); }
void AnalogJoystic::calibrateCenter() {
    calibrateVerticalCenter();
    calibrateHorizontalCenter();
}

uint16_t AnalogJoystic::readVertical() const { return analogRead(_vertPin) - _vertCenter; }
uint16_t AnalogJoystic::readHorizontal() const { return analogRead(_horzPin) - _horzCenter; }

bool AnalogJoystic::isInDeadzone(uint16_t vert, uint16_t horz) const {
    return abs(vert) < _deadzone && abs(horz) < _deadzone;
}
