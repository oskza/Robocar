#ifndef ENCODER_H
#define ENCODER_H
#include <Arduino.h>

class Encoder {
private:
  uint8_t _pin;
  volatile uint32_t _count;
public:
  Encoder(uint8_t pin);
  void init(void (*isr)());
  void tick();
  uint32_t getCount();
  void reset();
};
#endif
