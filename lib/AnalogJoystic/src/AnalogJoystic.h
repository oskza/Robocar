#ifndef ANALOG_JOYSTIC_H
#define ANALOG_JOYSTIC_H
#include <Arduino.h>

class AnalogJoystic {
private:
  uint8_t _vertPin, _horzPin;
  uint16_t _deadzone, _vertCenter, _horzCenter;
public:
  AnalogJoystic(uint8_t vertPin, uint8_t horzPin, uint16_t deadzone = 80);

  void calibrateCenter();
  void calibrateVerticalCenter();
  void calibrateHorizontalCenter();

  uint16_t readVertical() const;
  uint16_t getVerticalCenter() const;
  uint16_t getVerticalDeadzoneMin() const;
  uint16_t getVerticalDeadzoneMax() const;

  uint16_t readHorizontal() const;
  uint16_t getHorizontalCenter() const;
  uint16_t getHorizontalDeadzoneMin() const;
  uint16_t getHorizontalDeadzoneMax() const;

  bool isInDeadzone(uint16_t vert, uint16_t horz) const;

  bool isVerticalInDeadzone(uint16_t val) const;
  bool isVerticalBelowDeadzone(uint16_t val) const;
  bool isVerticalAboveDeadzone(uint16_t val) const;

  bool isHorizontalInDeadzone(uint16_t val) const;
  bool isHorizontalBelowDeadzone(uint16_t val) const;
  bool isHorizontalAboveDeadzone(uint16_t val) const;
};
#endif
