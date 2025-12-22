#ifndef ANALOG_JOYSTIC_H
#define ANALOG_JOYSTIC_H
#include <Arduino.h>

class AnalogJoystic {
private:
  uint8_t _vertPin;
  uint8_t _horzPin;
  uint16_t _vertCenter;
  uint16_t _horzCenter;
public:
  AnalogJoystic(uint8_t vertPin, uint8_t horzPin);
  void calibrateVerticalCenter();
  void calibrateHorizontalCenter();
  void calibrateCenter();
  uint16_t readVertical() const;
  uint16_t readHorizontal() const;
  uint16_t getVerticalCenter() const;
  uint16_t getHorizontalCenter() const;
};
#endif
