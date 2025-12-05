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

uint16_t AnalogJoystic::readVerticalRaw() const { return analogRead(_vertPin); }

int16_t AnalogJoystic::readVerticalPWM() const { 
    uint16_t raw = readVerticalRaw();
    return (raw < _vertCenter) 
            ? map(raw, _vertCenter, 0, 0, 255)
            : map(raw, _vertCenter, 4095, 0, -255);
}

uint16_t AnalogJoystic::readHorizontalRaw() const { return analogRead(_horzPin); }

int16_t AnalogJoystic::readHorizontalPWM() const { 
    uint16_t raw = readHorizontalRaw();
    return (raw < _vertCenter) 
            ? map(raw, _vertCenter, 0, 0, 255)
            : map(raw, _vertCenter, 4095, 0, -255);
}

bool AnalogJoystic::isInDeadzone(int16_t vert, int16_t horz) const {
    return abs(vert) <= _deadzone && abs(horz) <= _deadzone;
}
