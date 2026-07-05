#include "DifferentialDrive.h"

DifferentialDrive::DifferentialDrive(WheelOutputController &leftWheel, WheelOutputController &rightWheel)
    : _leftWheel(leftWheel), _rightWheel(rightWheel) {}

void DifferentialDrive::begin(
    uint8_t acceleration,
    uint32_t pwmFrequency,
    uint8_t leftMinEffectivePwm,
    uint8_t rightMinEffectivePwm
) {
    _leftWheel.begin(acceleration, pwmFrequency, leftMinEffectivePwm);
    _rightWheel.begin(acceleration, pwmFrequency, rightMinEffectivePwm);
}

bool DifferentialDrive::isStopped() const { return _leftWheel.isStopped() && _rightWheel.isStopped(); }

int16_t DifferentialDrive::getLeftOutput() const { return _leftWheel.getCurrentOutput(); }

int16_t DifferentialDrive::getRightOutput() const { return _rightWheel.getCurrentOutput(); }

void DifferentialDrive::drive(int16_t velocity, int16_t turn) {
    velocity = constrain(velocity, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
    turn = constrain(turn, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
    int16_t rightOutput = velocity + turn;
    int16_t leftOutput  = velocity - turn;
    int16_t maxMagnitude = max(abs(leftOutput), abs(rightOutput));
    if (maxMagnitude > MotorDriver::MAX_OUTPUT) {
        leftOutput  = (leftOutput  * MotorDriver::MAX_OUTPUT) / maxMagnitude;
        rightOutput = (rightOutput * MotorDriver::MAX_OUTPUT) / maxMagnitude;
    }
    _leftWheel.setTargetOutput(leftOutput);
    _rightWheel.setTargetOutput(rightOutput);
}

void DifferentialDrive::update() {
    _leftWheel.update();
    _rightWheel.update();
}

void DifferentialDrive::stop() {
    _leftWheel.stop();
    _rightWheel.stop();
}

void DifferentialDrive::brake() {
    _leftWheel.brake();
    _rightWheel.brake();
}
