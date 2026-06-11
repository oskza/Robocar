#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel)
                : _pwmPin(pwmPin), _inNormPin(inNormPin), _inRevPin(inRevPin),
                    _pwmChannel(pwmChannel), _minPWM(0), _pwm(0), 
                    _direction(MotorDirection::MOTOR_DIR_NONE) {}

uint8_t Motor::scalePWM(uint8_t pwm, uint8_t min, uint8_t max) {
    return (pwm != 0) ? (uint8_t)(((int32_t)pwm * (max - min)) / 255 + min) : 0;
}

void Motor::_applyDirection(uint8_t dir) {
    _direction = dir;
    switch (dir) {
        case MotorDirection::MOTOR_DIR_NONE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, LOW);
            break;
        case MotorDirection::MOTOR_DIR_NORMAL:
            digitalWrite(_inNormPin, HIGH);
            digitalWrite(_inRevPin, LOW);
            break;
        case MotorDirection::MOTOR_DIR_REVERSE:
            digitalWrite(_inNormPin, LOW);
            digitalWrite(_inRevPin, HIGH);
            break;
    }
}

void Motor::_applyPWM(uint8_t pwm) {
    _pwm = pwm;
    ledcWrite(_pwmChannel, pwm); 
}

void Motor::init(uint32_t freq, uint8_t res, uint8_t minPWM) {
    pinMode(_inNormPin, OUTPUT);
    pinMode(_inRevPin, OUTPUT);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    _minPWM = minPWM;
    _applyPWM(0);
    _applyDirection(MotorDirection::MOTOR_DIR_NONE);
}

void Motor::stop() {
    if (_pwm == 0)
        return;
    _applyPWM(0);
    _applyDirection(MotorDirection::MOTOR_DIR_NONE);
}

void Motor::run(int16_t pwm) {
    if (pwm == 0) {
        stop();
        return;
    }
    pwm = constrain(pwm, -255, 255);
    if(pwm < 0) {
        setPWM(-pwm);
        setDirection(MotorDirection::MOTOR_DIR_REVERSE);
        return;
    }
    setPWM(pwm);
    setDirection(MotorDirection::MOTOR_DIR_NORMAL);
}

void Motor::setDirection(uint8_t dir) {
    if(_direction == dir)
        return;
    _applyDirection(dir);
}

uint8_t Motor::getPWM() const { return _pwm; }

void Motor::setPWM(uint8_t pwm) {
    if (_minPWM > 0) 
        pwm = scalePWM(pwm, _minPWM, 255);
    if (pwm == _pwm)
        return;
    _applyPWM(pwm);
}

void Motor::setMinPWM(uint8_t pwm) { _minPWM = pwm; }
