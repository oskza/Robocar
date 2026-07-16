#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin, uint8_t pwmChannel)
    : _pwmPin(pwmPin),
      _in1Pin(in1Pin),
      _in2Pin(in2Pin),
      _pwmChannel(pwmChannel),
      _minEffectivePwm(0),
      _output(0) {}

uint8_t MotorDriver::_scalePwm(uint8_t pwm) const {
    return ((pwm == 0 || _minEffectivePwm == 0))
        ? pwm
        : (uint8_t)(((uint16_t)
            pwm * (MAX_OUTPUT - _minEffectivePwm)) / MAX_OUTPUT + _minEffectivePwm);
}

void MotorDriver::_applyCoast() {
    digitalWrite(_in1Pin, LOW);
    digitalWrite(_in2Pin, LOW);
}

void MotorDriver::_applyBrake() {
    digitalWrite(_in1Pin, HIGH);
    digitalWrite(_in2Pin, HIGH);
}

void MotorDriver::_applyForward() {
    digitalWrite(_in1Pin, HIGH);
    digitalWrite(_in2Pin, LOW);
}

void MotorDriver::_applyReverse() {
    digitalWrite(_in1Pin, LOW);
    digitalWrite(_in2Pin, HIGH);
}

void MotorDriver::begin(uint32_t pwmFrequency, uint8_t minEffectivePwm) {
    _minEffectivePwm = constrain(minEffectivePwm, 0, MAX_OUTPUT);
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
    ledcSetup(_pwmChannel, pwmFrequency, PWM_RESOLUTION);
    ledcAttachPin(_pwmPin, _pwmChannel);
    _output = 0;
    ledcWrite(_pwmChannel, 0);
    _applyCoast();
}

void MotorDriver::stop() {
    _output = 0;
    ledcWrite(_pwmChannel, 0);
    _applyCoast();
}

void MotorDriver::brake() {
    _output = 0;
    _applyBrake();
    ledcWrite(_pwmChannel, MAX_OUTPUT);
}

bool MotorDriver::isStopped() const { return _output == 0; }

int16_t MotorDriver::getOutput() const { return _output; }

void MotorDriver::setOutput(int16_t output) {
    output = constrain(output, -MAX_OUTPUT, MAX_OUTPUT);
    if (_output == output)
        return;
    _output = output;
    if (_output == 0) {
        ledcWrite(_pwmChannel, 0);
        _applyCoast();
        return;
    }
    uint8_t hwPwm = _scalePwm((uint8_t)abs(_output));
    if (_output > 0)
        _applyForward();
    else
        _applyReverse();
    ledcWrite(_pwmChannel, hwPwm);
}
