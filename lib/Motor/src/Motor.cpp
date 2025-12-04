#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, uint8_t pwmChannel, float corr) 
        : _pwmPin(pwmPin), _fwdPin(fwdPin), _bckPin(bckPin), _pwmChannel(pwmChannel), _corr(corr) {}

uint8_t Motor::_correctPWM(uint8_t pwm) {
    int res = (int)round(pwm * _corr);
    return (res > MOTOR_MAX_PWM)
            ? MOTOR_MAX_PWM
            : (res < MOTOR_MIN_PWM)
                ? MOTOR_MIN_PWM
                : (uint8_t)res;
}

void Motor::init(uint32_t freq, uint8_t res) {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_bckPin, OUTPUT);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    stop();
}

void Motor::stop() {
    ledcWrite(_pwmChannel, 0);
    digitalWrite(_fwdPin, LOW);
    digitalWrite(_bckPin, LOW);
}

void Motor::forward(uint8_t pwm) {
    ledcWrite(_pwmChannel, _correctPWM(pwm));
    digitalWrite(_fwdPin, HIGH);
    digitalWrite(_bckPin, LOW);
}

void Motor::backward(uint8_t pwm) {
    ledcWrite(_pwmChannel, _correctPWM(pwm));
    digitalWrite(_fwdPin, LOW);
    digitalWrite(_bckPin, HIGH);
}

void Motor::rotate(int16_t pwm) {
    if(pwm >= 0) 
        forward(_correctPWM(pwm)); 
    else 
        backward(_correctPWM(-pwm));
}
