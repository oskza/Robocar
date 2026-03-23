#include "AnalogJoystic.h"

AnalogJoystic::AnalogJoystic(uint8_t vertPin, uint8_t horzPin) 
                                : _vertPin(vertPin), _horzPin(horzPin), 
                                    _vertCenter(ANALOG_JOYSTIC_MAX_VALUE / 2), 
                                    _horzCenter(ANALOG_JOYSTIC_MAX_VALUE / 2),
                                    _deadzone(80) {}

void AnalogJoystic::calibrateVerticalCenter() { _vertCenter = readVerticalRaw(); }

void AnalogJoystic::calibrateHorizontalCenter() { _horzCenter = readHorizontalRaw(); }

uint16_t AnalogJoystic::getVerticalCenter() const { return _vertCenter; }

uint16_t AnalogJoystic::getHorizontalCenter() const { return _horzCenter; }

uint16_t AnalogJoystic::readVerticalRaw() const { return analogRead(_vertPin); }

uint16_t AnalogJoystic::readHorizontalRaw() const { return analogRead(_horzPin); }

uint16_t AnalogJoystic::getDeadzone() const { return _deadzone; }

void AnalogJoystic::setDeadzone(uint16_t deadzone) { _deadzone = deadzone; }
