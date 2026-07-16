#include "WheelActuator.h"

WheelActuator::WheelActuator(MotorDriver &motor)
    : _motor(motor),
      _targetOutput(0),
      _currentOutput(0),
      _acceleration(DEFAULT_ACCELERATION) {}

void WheelActuator::begin(uint8_t acceleration, uint32_t pwmFrequency, uint8_t minEffectivePwm) {
    _motor.begin(pwmFrequency, minEffectivePwm);
    setAcceleration(acceleration);
    _targetOutput = 0;
    _currentOutput = 0;
}

void WheelActuator::update() {
    if (_acceleration == 0) {
        _currentOutput = _targetOutput;
    } else if (_currentOutput < _targetOutput) {
        _currentOutput += _acceleration;
        if (_currentOutput > _targetOutput)
            _currentOutput = _targetOutput;
    } else if (_currentOutput > _targetOutput) {
        _currentOutput -= _acceleration;
        if (_currentOutput < _targetOutput)
            _currentOutput = _targetOutput;
    }
    _motor.setOutput(_currentOutput);
}

void WheelActuator::stop() { _targetOutput = 0; }

void WheelActuator::brake() {
    _targetOutput = 0;
    _currentOutput = 0;
    _motor.brake();
}

bool WheelActuator::isStopped() const {
    return _targetOutput == 0 && _currentOutput == 0;
}

int16_t WheelActuator::getCurrentOutput() const { return _currentOutput; }

int16_t WheelActuator::getTargetOutput() const { return _targetOutput; }

void WheelActuator::setTargetOutput(int16_t output) {
    _targetOutput = constrain(output, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
}

uint8_t WheelActuator::getAcceleration() const { return _acceleration; }

void WheelActuator::setAcceleration(uint8_t acceleration) {
    _acceleration = min(acceleration, MotorDriver::MAX_OUTPUT);
}
