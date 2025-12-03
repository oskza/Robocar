#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t fwdPin, uint8_t bckPin, uint8_t pwmChannel) 
        : _pwmPin(pwmPin), _fwdPin(fwdPin), _bckPin(bckPin), _pwmChannel(pwmChannel) {}
        
void Motor::init(uint32_t freq, uint8_t res) {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_bckPin, OUTPUT);
    ledcSetup(_pwmChannel, freq, res);
    ledcAttachPin(_pwmPin, _pwmChannel);
    stop();
}

void Motor::forward(uint8_t pwm) {
    ledcWrite(_pwmChannel, pwm);
    digitalWrite(_fwdPin, HIGH);
    digitalWrite(_bckPin, LOW);
}

void Motor::backward(uint8_t pwm) {
    ledcWrite(_pwmChannel, pwm);
    digitalWrite(_fwdPin, LOW);
    digitalWrite(_bckPin, HIGH);
}

void Motor::stop() {
    ledcWrite(_pwmChannel, 0);
    digitalWrite(_fwdPin, LOW);
    digitalWrite(_bckPin, LOW);
}
