#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel)
                : _pwmPin(pwmPin), _inNormPin(inNormPin), _inRevPin(inRevPin), _pwmChannel(pwmChannel), 
                    _pwm(0), _minPwm(0), _direction(MotorDirection::NONE) {}

uint8_t Motor::scalePwm(uint8_t pwm, uint8_t minVal, uint8_t maxVal) {
    return (!pwm) ? 0 : (uint8_t)(((int32_t)pwm * (maxVal - minVal)) / 255 + minVal);
}

void Motor::_applyDirection(MotorDirection dir) {
    switch (dir) {
        case MotorDirection::NONE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, LOW);
            break;
        case MotorDirection::NORMAL:
            digitalWrite(_inNormPin, HIGH);
            digitalWrite(_inRevPin, LOW);
            break;
        case MotorDirection::REVERSE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, HIGH);
            break;
    }
}

void Motor::init(uint32_t freq, uint8_t res, uint8_t minPwm) {
    pinMode(_inNormPin, OUTPUT);
    pinMode(_inRevPin, OUTPUT);
    digitalWrite(_inNormPin, LOW);
    digitalWrite(_inRevPin, LOW);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    ledcWrite(_pwmChannel, 0);
    _pwm = 0;
    _minPwm = minPwm;
    _direction = MotorDirection::NONE;
}

void Motor::stop() {
    setPwm(0);
    setDirection(MotorDirection::NONE);
}

void Motor::run(int16_t pwm) {
    if (pwm == 0) {
        stop();
        return;
    }
    pwm = constrain(pwm, -255, 255);
    if(pwm < 0) {
        setPwm(-pwm);
        setDirection(MotorDirection::REVERSE);
        return;
    }
    setPwm(pwm);
    setDirection(MotorDirection::NORMAL);
}

void Motor::normal(uint8_t pwm) {
    if (pwm == 0) {
        stop();
        return;
    }
    setPwm(pwm);
    setDirection(MotorDirection::NORMAL);
}

void Motor::reverse(uint8_t pwm) {
    if (pwm == 0) {
        stop();
        return;
    }
    setPwm(pwm);
    setDirection(MotorDirection::REVERSE);
}

uint8_t Motor::getPwm() const { return _pwm; }

void Motor::setPwm(uint8_t pwm) {
    if (_minPwm > 0) 
        pwm = scalePwm(pwm, _minPwm);
    if (_pwm == pwm)
        return;
    _pwm = pwm;
    ledcWrite(_pwmChannel, pwm); 
}

MotorDirection Motor::getDirection() const { return _direction; }

void Motor::setDirection(MotorDirection dir) {
    if(_direction == dir)
        return;
    _direction = dir;
    _applyDirection(dir);
}

void Motor::setMinPwm(uint8_t pwm) { _minPwm = pwm; }
