#ifndef ANALOG_JOYSTIC_H
#define ANALOG_JOYSTIC_H
#include <Arduino.h>

class AnalogJoystic {
private:
  uint8_t _vertPin, _horzPin;
  uint16_t _deadzone, _vertCenter, _horzCenter;
public:
  AnalogJoystic(uint8_t vertPin, uint8_t horzPin, uint16_t deadzone = 80);

  void calibrateVerticalCenter();
  void calibrateHorizontalCenter();
  void calibrateCenter();

  uint16_t readVertical() const;
  uint16_t readHorizontal() const;

  bool isInDeadzone(uint16_t vert, uint16_t horz) const;
};
#endif
