#include "MotorsX2.h"

MotorsX2::MotorsX2(Motor *motorR, Motor *motorL) 
                    : _motorR(motorR), _motorL(motorL) {}

void MotorsX2::init(uint32_t freq, uint8_t res) {
    _motorR->init(freq, res);
    _motorL->init(freq, res);
}

void MotorsX2::stop() {
    _motorR->stop(); 
    _motorL->stop();
}

void MotorsX2::move(int16_t velocity, int16_t turn) {
    int16_t pwmR = constrain(velocity + turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    int16_t pwmL = constrain(velocity - turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    _motorR->rotate(pwmR); 
    _motorL->rotate(pwmL); 
}
