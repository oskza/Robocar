#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

enum MotorDirection : uint8_t {
  MOTOR_DIR_NONE, MOTOR_DIR_NORMAL, MOTOR_DIR_REVERSE
};

class Motor {
private:
  uint8_t _pwmPin;
  uint8_t _inNormPin;
  uint8_t _inRevPin;
  uint8_t _pwmChannel;
  uint8_t _pwm;
  uint8_t _minPwm;
  MotorDirection _direction;
  void _applyDirection(MotorDirection dir);
public:
  Motor(uint8_t pwmPin, uint8_t inNormPin, 
          uint8_t inRevPin, uint8_t pwmChannel);
  static uint8_t scalePwm(uint8_t pwm, uint8_t minVal, uint8_t maxVal);
  void init(uint32_t freq, uint8_t res, uint8_t minPwm = 0);
  void stop();
  void run(int16_t pwm);
  void normal(uint8_t pwm);
  void reverse(uint8_t pwm);
  uint8_t getPwm() const;
  void setPwm(uint8_t pwm);
  MotorDirection getDirection() const;
  void setDirection(MotorDirection dir);
  void setMinPwm(uint8_t pwm);
};
#endif
