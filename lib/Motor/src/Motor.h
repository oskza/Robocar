#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

#define MOTOR_MIN_PWM 110
#define MOTOR_MAX_PWM 255

class Motor {
private:
  uint8_t _pwmPin, _fwdPin, _bckPin, _pwmChannel;
  float _corr;
  uint8_t _correctPWM(uint8_t pwm);
public:
  Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, uint8_t pwmChannel, float corr = 1);
  void init(uint32_t freq, uint8_t res);
  void stop();
  void forward(uint8_t pwm);
  void backward(uint8_t pwm);
  void rotate(int16_t pwm);
};
#endif
