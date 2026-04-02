#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

#define MOTOR_MAX_PWM     255

#define MOTOR_DEFAULT_RES 8

#define MOTOR_DIR_NONE    0
#define MOTOR_DIR_NORMAL  1
#define MOTOR_DIR_REVERSE 2

class Motor {
private:
  uint8_t _pwmPin;
  uint8_t _inNormPin;
  uint8_t _inRevPin;
  uint8_t _pwmChannel;
  uint8_t _pwmResolution;
  uint8_t _minPWM;
  uint8_t _pwm;
  uint8_t _direction;
  void _writeDirection(uint8_t dir);
  void _writePWM(uint8_t pwm);
public:
  Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel);
  static uint8_t scalePWM(uint8_t pwm, uint8_t minPWM, uint8_t maxPWM, uint8_t res);
  void init(uint32_t freq, uint8_t res, uint8_t minPWM);
  void stop();
  void run(int16_t pwm);
  void setDirection(uint8_t dir);
  uint8_t getPWM() const;
  void setPWM(uint8_t pwm);
  void setMinPWM(uint8_t pwm);
};
#endif
