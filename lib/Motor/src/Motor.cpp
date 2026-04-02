#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t inNormPin, uint8_t inRevPin, uint8_t pwmChannel)
                : _pwmPin(pwmPin), _inNormPin(inNormPin), _inRevPin(inRevPin),
                    _pwmChannel(pwmChannel), _pwmResolution(MOTOR_DEFAULT_RES),
                    _minPWM(0), _pwm(0), _direction(MOTOR_DIR_NONE) {}

void Motor::_writeDirection(uint8_t dir) {
    _direction = dir;
    switch (dir) {
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

void Motor::_writePWM(uint8_t pwm) {
    _pwm = pwm;
    ledcWrite(_pwmChannel, pwm); 
}

uint8_t Motor::scalePWM(uint8_t pwm, uint8_t minPWM, uint8_t maxPWM, uint8_t res) {
    return (pwm != 0)
            ? (uint8_t)(((int32_t)pwm * (maxPWM - minPWM)) / 255 + minPWM)
            : 0;
}

void Motor::init(uint32_t freq, uint8_t res, uint8_t minPWM) {
    pinMode(_inNormPin, OUTPUT);
    pinMode(_inRevPin, OUTPUT);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    _pwmResolution = res;
    _minPWM = minPWM;
    _writePWM(0);
    _writeDirection(MOTOR_DIR_NONE);
}

void Motor::stop() {
    if (_pwm == 0)
        return;
    _writePWM(0);
    _writeDirection(MOTOR_DIR_NONE);
}

void Motor::run(int16_t pwm) {
    if (pwm == 0) {
        stop();
        return;
    }
    pwm = constrain(pwm, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    if(pwm < 0) {
        setPWM(-pwm);
        setDirection(MOTOR_DIR_REVERSE);
        return;
    }
    setPWM(pwm);
    setDirection(MOTOR_DIR_NORMAL);
}

void Motor::setDirection(uint8_t dir) {
    if(_direction == dir)
        return;
    _writeDirection(dir);
}

uint8_t Motor::getPWM() const { return _pwm; }

void Motor::setPWM(uint8_t pwm) {
    if (pwm > 0 && _minPWM > 0) 
        pwm = scalePWM(pwm, _minPWM, MOTOR_MAX_PWM, _pwmResolution);
    if (pwm == _pwm)
        return;
    _writePWM(pwm);
}

void Motor::setMinPWM(uint8_t pwm) { _minPWM = pwm; }
