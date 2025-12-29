#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

#define MOTOR_DIR_NONE    0
#define MOTOR_DIR_NORMAL  1
#define MOTOR_DIR_REVERSE 2

#define MOTOR_MIN_PWM     110
#define MOTOR_MAX_PWM     255

class Motor {
private:
  uint8_t _pwmPin;
  uint8_t _inNormPin;
  uint8_t _inRevPin;
  uint8_t _pwmChannel;
  float _correction;
  uint8_t _direction;
  uint8_t _pwm;
  void _writeDirection();
  uint8_t _applyCorrection(uint8_t pwm) const;
public:
  Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel);
  Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel, float correction);
  void init(uint32_t freq, uint8_t res);
  void stop();
  uint8_t getPWM() const;
  void setPWM(uint8_t pwm);
  void setSignedPWM(int16_t pwm);
  void normal(uint8_t pwm);
  void reverse(uint8_t pwm);
  void setDirection(uint8_t dir);
};
#endif
