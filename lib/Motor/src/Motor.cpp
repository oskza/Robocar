#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel)
                : Motor(pwmPin, inNormPin, inRevPin, pwmChannel, 1) {}

Motor::Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel, float correction) 
                : _pwmPin(pwmPin), _inNormPin(inNormPin), _inRevPin(inRevPin),
                    _pwmChannel(pwmChannel), _correction(correction), _direction(MOTOR_DIR_NONE) {}

void Motor::_writeDirection() {
    switch (_direction) {
        case MOTOR_DIR_NONE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, LOW);
            break;
        case MOTOR_DIR_NORMAL:
            digitalWrite(_inNormPin, HIGH);
            digitalWrite(_inRevPin, LOW);
            break;
        case MOTOR_DIR_REVERSE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, HIGH);
            break;
    }
}

void Motor::_setDirection(uint8_t dir) {
    if(_direction == dir)
        return;
    _direction = dir;
    _writeDirection();
}

uint8_t Motor::_applyCorrection(uint8_t pwm) const {
    int16_t res = (int16_t)round(pwm * _correction);
    return (res > MOTOR_MAX_PWM)
            ? MOTOR_MAX_PWM
            : (res < MOTOR_MIN_PWM)
                ? MOTOR_MIN_PWM
                : (uint8_t)res;
}

void Motor::init(uint32_t freq, uint8_t res) {
    pinMode(_inNormPin, OUTPUT);
    pinMode(_inRevPin, OUTPUT);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    stop();
}

void Motor::stop() {
    ledcWrite(_pwmChannel, 0);
    _setDirection(MOTOR_DIR_NONE);
}

void Motor::setPWM(uint8_t pwm) { ledcWrite(_pwmChannel, _applyCorrection(pwm)); }

void Motor::normal(uint8_t pwm) {
    setPWM(pwm);
    setDirNormal();
}

void Motor::reverse(uint8_t pwm) {
    setPWM(pwm);
    setDirReverse();
}

void Motor::setSignedPWM(int16_t pwm) {
    if(pwm > 0) {
        normal(pwm); 
        return;
    }
    if(pwm < 0) {
        reverse(-pwm);
        return;
    }
    stop();
}

bool Motor::isStopped() const { return _direction == MOTOR_DIR_NONE; }

bool Motor::isDirNormal() const { return _direction == MOTOR_DIR_NORMAL; }

bool Motor::isDirReverse() const { return _direction == MOTOR_DIR_REVERSE; }

void Motor::setDirNormal() { _setDirection(MOTOR_DIR_NORMAL); }

void Motor::setDirReverse() { _setDirection(MOTOR_DIR_REVERSE); }
