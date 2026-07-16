#include "DifferentialDrive.h"

DifferentialDrive::DifferentialDrive(WheelActuator &leftWheel, WheelActuator &rightWheel)
    : _leftWheel(leftWheel), _rightWheel(rightWheel) {}

void DifferentialDrive::_normalizeOutputs(int16_t &leftOutput, int16_t &rightOutput) {
    const int16_t maxMagnitude = max(abs(leftOutput), abs(rightOutput));
    if (maxMagnitude <= MAX_OUTPUT)
        return;
    leftOutput = (int32_t(leftOutput) * MAX_OUTPUT) / maxMagnitude;
    rightOutput = (int32_t(rightOutput) * MAX_OUTPUT) / maxMagnitude;
}

void DifferentialDrive::begin(
    uint8_t acceleration,
    uint32_t pwmFrequency,
    uint8_t leftMinEffectivePwm,
    uint8_t rightMinEffectivePwm
) {
    _leftWheel.begin(acceleration, pwmFrequency, leftMinEffectivePwm);
    _rightWheel.begin(acceleration, pwmFrequency, rightMinEffectivePwm);
}

void DifferentialDrive::update() {
    _leftWheel.update();
    _rightWheel.update();
}

void DifferentialDrive::drive(int16_t velocity, int16_t turn) {
    velocity = constrain(velocity, -MAX_OUTPUT, MAX_OUTPUT);
    turn = constrain(turn, -MAX_OUTPUT, MAX_OUTPUT);
    int16_t leftOutput = velocity - turn;
    int16_t rightOutput = velocity + turn;
    _normalizeOutputs(leftOutput, rightOutput);
    _leftWheel.setTargetOutput(leftOutput);
    _rightWheel.setTargetOutput(rightOutput);
}

void DifferentialDrive::stop() {
    _leftWheel.stop();
    _rightWheel.stop();
}

void DifferentialDrive::brake() {
    _leftWheel.brake();
    _rightWheel.brake();
}

bool DifferentialDrive::isStopped() const { return _leftWheel.isStopped() && _rightWheel.isStopped(); }

int16_t DifferentialDrive::getLeftOutput() const { return _leftWheel.getCurrentOutput(); }

int16_t DifferentialDrive::getRightOutput() const { return _rightWheel.getCurrentOutput(); }
