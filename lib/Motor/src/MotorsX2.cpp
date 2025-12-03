#include "MotorsX2.h"

MotorsX2::MotorsX2(Motor *motorR, Motor *motorL) 
                    : _motorR(motorR), _motorL(motorL) {}

void MotorsX2::init(uint32_t freq, uint8_t res) {
    _motorR->init(freq, res);
    _motorL->init(freq, res);
}

void MotorsX2::moveForward(uint8_t pwm) { 
    _motorR->forward(pwm);
    _motorL->forward(pwm); 
}

void MotorsX2::moveBackward(uint8_t pwm) { 
    _motorR->backward(pwm); 
    _motorL->backward(pwm); 
}

void MotorsX2::turnLeft(uint8_t pwm) {
    _motorL->backward(pwm);
    _motorR->forward(pwm);
}

void MotorsX2::turnRight(uint8_t pwm) {
    _motorR->backward(pwm);
    _motorL->forward(pwm);
}

void MotorsX2::stop() { 
    _motorR->stop(); 
    _motorL->stop(); 
}
