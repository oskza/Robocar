#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

#define MOTOR_DIR_NONE      0
#define MOTOR_DIR_FORWARD   1
#define MOTOR_DIR_BACKWARD  2

#define MOTOR_MIN_PWM       110
#define MOTOR_MAX_PWM       255

class Motor {
private:
  uint8_t _pwmPin;
  uint8_t _fwdPin;
  uint8_t _bckPin;
  uint8_t _pwmChannel;
  uint8_t _dir;
  float _corr;
  uint8_t _correctPWM(uint8_t pwm);
  void _writeDir(uint8_t dir);
  void _setDir(uint8_t dir);
public:
  Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, 
          uint8_t pwmChannel, float corr = 1);
  static bool validateDirection(uint8_t dir);
  void init(uint32_t freq, uint8_t res);
  void stop();
  void forward(uint8_t pwm);
  void backward(uint8_t pwm);
  void rotate(int16_t pwm);
  uint8_t getDirection() const;
  void setDirection(uint8_t dir);
};
#endif
