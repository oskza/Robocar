#ifndef MOTORS_X2_H
#define MOTORS_X2_H
#include "Motor.h"

class MotorsX2 {
private:
  Motor *_motorR;
  Motor *_motorL;
public:
  MotorsX2(Motor *motorR, Motor *motorL);
  void init(uint32_t freq, uint8_t res);
  void stop();
  void moveForward(uint8_t pwm);
  void moveBackward(uint8_t pwm);
  void turnLeft(uint8_t pwm);
  void turnRight(uint8_t pwm);
  void moveDifferential(int16_t velocity, int16_t turn);
};
#endif
