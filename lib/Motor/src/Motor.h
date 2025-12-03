#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
private:
  uint8_t _pwmPin, _fwdPin, _bckPin, _pwmChannel;
public:
  Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, uint8_t pwmChannel);
  void init(uint32_t freq, uint8_t res);
  void forward(uint8_t pwm);
  void backward(uint8_t pwm);
  void stop();
};
#endif
