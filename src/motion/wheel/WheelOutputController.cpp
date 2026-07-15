#include "WheelOutputController.h"

WheelOutputController::WheelOutputController(MotorDriver &motor)
    : _motor(motor),
      _targetOutput(0),
      _currentOutput(0),
      _acceleration(5) {}

void WheelOutputController::begin(uint8_t acceleration, uint32_t pwmFrequency, uint8_t minEffectivePwm) {
    _motor.begin(pwmFrequency, minEffectivePwm);
    setAcceleration(acceleration);
    _targetOutput = 0;
    _currentOutput = 0;
}

bool WheelOutputController::isStopped() const { return _targetOutput == 0 && _currentOutput == 0; }

uint8_t WheelOutputController::getAcceleration() const { return _acceleration; }

void WheelOutputController::setAcceleration(uint8_t acceleration) {
    _acceleration = min(acceleration, MotorDriver::MAX_OUTPUT);
}

int16_t WheelOutputController::getCurrentOutput() const { return _currentOutput; }

int16_t WheelOutputController::getTargetOutput() const { return _targetOutput; }

void WheelOutputController::setTargetOutput(int16_t output) {
    _targetOutput = constrain(output, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
}

void WheelOutputController::update() {
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

void WheelOutputController::stop() { _targetOutput = 0; }

void WheelOutputController::brake() {
    _targetOutput = 0;
    _currentOutput = 0;
    _motor.brake();
}
