#include "AnalogJoystic.h"

AnalogJoystic::AnalogJoystic(uint8_t vertPin, uint8_t horzPin) 
                                : _vertPin(vertPin), _horzPin(horzPin), 
                                    _vertCenter(2048), _horzCenter(2048) {}

void AnalogJoystic::calibrateVerticalCenter() { _vertCenter = analogRead(_vertPin); }

void AnalogJoystic::calibrateHorizontalCenter() { _horzCenter = analogRead(_horzPin); }

void AnalogJoystic::calibrateCenter() { 
    calibrateVerticalCenter(); 
    calibrateHorizontalCenter(); 
}

uint16_t AnalogJoystic::readVertical() const { return analogRead(_vertPin); }

uint16_t AnalogJoystic::readHorizontal() const { return analogRead(_horzPin); }

uint16_t AnalogJoystic::getVerticalCenter() const { return _vertCenter; }

uint16_t AnalogJoystic::getHorizontalCenter() const { return _horzCenter; }
