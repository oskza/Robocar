#ifndef ANALOG_JOYSTIC_H
#define ANALOG_JOYSTIC_H
#include <Arduino.h>

class AnalogJoystic {
private:
  uint8_t _vertPin;
  uint8_t _horzPin;
  uint16_t _vertCenter;
  uint16_t _horzCenter;
  uint16_t _deadzone;
public:
  AnalogJoystic(uint8_t vertPin, uint8_t horzPin, uint16_t deadzone = 80);
  void calibrateVerticalCenter();
  void calibrateHorizontalCenter();
  void calibrateCenter();
  uint16_t readVerticalRaw() const;
  int16_t readVerticalPWM() const;
  uint16_t readHorizontalRaw() const;
  int16_t readHorizontalPWM() const;
  bool isInDeadzone(int16_t vert, int16_t horz) const;
};
#endif
