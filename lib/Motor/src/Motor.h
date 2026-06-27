#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

enum MotorDirection : uint8_t { NONE, NORMAL, REVERSE };

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
protected:
  void setPwm(uint8_t pwm);
  void setDirection(MotorDirection dir);
public:
  Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel);
  void init(uint32_t freq, uint8_t res, uint8_t minPwm = 0);
  void stop();
  void run(int16_t pwm);
  uint8_t getPwm() const;
  MotorDirection getDirection() const;
  void setMinPwm(uint8_t pwm);
  static inline uint8_t scalePwm(uint8_t pwm, uint8_t minVal, uint8_t maxVal = 255) {
    return (!pwm) ? 0 : (uint8_t)((uint16_t)pwm * (maxVal - minVal) / 255 + minVal);
  }
};
#endif
