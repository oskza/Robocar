#ifndef ANALOG_JOYSTIC_H
#define ANALOG_JOYSTIC_H
#include <Arduino.h>

#define ANALOG_JOYSTIC_MAX_VALUE 4095
#define ANALOG_JOYSTIC_MIN_VALUE 0

class AnalogJoystic {
private:
  uint8_t _vertPin;
  uint8_t _horzPin;
  uint16_t _vertCenter;
  uint16_t _horzCenter;
  uint16_t _deadzone;
public:
  AnalogJoystic(uint8_t vertPin, uint8_t horzPin);
  void calibrateVerticalCenter();
  void calibrateHorizontalCenter();
  uint16_t readVerticalRaw() const;
  uint16_t readHorizontalRaw() const;
  uint16_t getVerticalCenter() const;
  uint16_t getHorizontalCenter() const;
  uint16_t getDeadzone() const;
  void setDeadzone(uint16_t deadzone);
};
#endif
