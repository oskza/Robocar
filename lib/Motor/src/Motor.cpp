#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, 
                uint8_t pwmChannel, float corr) 
            : _pwmPin(pwmPin), _fwdPin(fwdPin), _bckPin(bckPin),
                _pwmChannel(pwmChannel), _corr(corr) {}

uint8_t Motor::_correctPWM(uint8_t pwm) {
    int res = (int)round(pwm * _corr);
    return (res > MOTOR_MAX_PWM)
            ? MOTOR_MAX_PWM
            : (res < MOTOR_MIN_PWM)
                ? MOTOR_MIN_PWM
                : (uint8_t)res;
}

void Motor::_writeDir(uint8_t dir) {
    switch (dir) {
        case MOTOR_DIR_NONE:
            digitalWrite(_fwdPin, LOW);
            digitalWrite(_bckPin, LOW);
            break;
        case MOTOR_DIR_FORWARD:
            digitalWrite(_fwdPin, HIGH);
            digitalWrite(_bckPin, LOW);
            break;
        case MOTOR_DIR_BACKWARD:
            digitalWrite(_fwdPin, LOW);
            digitalWrite(_bckPin, HIGH);
            break;
    }
}

void Motor::_setDir(uint8_t dir) {
    if(_dir == dir)
        return;
    _dir = dir;
    _writeDir(_dir);
}

bool Motor::validateDirection(uint8_t dir) {
    return dir == MOTOR_DIR_NONE 
            || dir == MOTOR_DIR_FORWARD 
            || dir == MOTOR_DIR_BACKWARD;
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
    _setDir(MOTOR_DIR_NONE);
}

void Motor::forward(uint8_t pwm) {
    ledcWrite(_pwmChannel, _correctPWM(pwm));
    _setDir(MOTOR_DIR_FORWARD);
}

void Motor::backward(uint8_t pwm) {
    ledcWrite(_pwmChannel, _correctPWM(pwm));
    _setDir(MOTOR_DIR_BACKWARD);
}

void Motor::rotate(int16_t pwm) {
    if(pwm > 0) {
        forward(pwm); 
        return;
    }
    if(pwm < 0) {
        backward(-pwm);
        return;
    }
    stop();
}

uint8_t Motor::getDirection() const { return _dir; }

void Motor::setDirection(uint8_t dir) {
    if(!validateDirection(dir))
        return;
    _setDir(_dir);
}
