#include "DifferentialDrive.h"

DifferentialDrive::DifferentialDrive(WheelOutputController &rightWheel, WheelOutputController &leftWheel)
    : _rightWheel(rightWheel), _leftWheel(leftWheel) {}

void DifferentialDrive::begin(uint8_t acceleration) {
    _rightWheel.begin(acceleration);
    _leftWheel.begin(acceleration);
}

bool DifferentialDrive::isStopped() const { return _rightWheel.isStopped() && _leftWheel.isStopped(); }

int16_t DifferentialDrive::getRightOutput() const { return _rightWheel.getCurrentOutput(); }

int16_t DifferentialDrive::getLeftOutput() const { return _leftWheel.getCurrentOutput(); }

void DifferentialDrive::drive(int16_t velocity, int16_t turn) {
    velocity = constrain(velocity, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
    turn = constrain(turn, -MotorDriver::MAX_OUTPUT, MotorDriver::MAX_OUTPUT);
    int16_t rightOutput = velocity + turn;
    int16_t leftOutput  = velocity - turn;
    int16_t maxMagnitude = max(abs(rightOutput), abs(leftOutput));
    if (maxMagnitude > MotorDriver::MAX_OUTPUT) {
        rightOutput = (rightOutput * MotorDriver::MAX_OUTPUT) / maxMagnitude;
        leftOutput  = (leftOutput  * MotorDriver::MAX_OUTPUT) / maxMagnitude;
    }
    _rightWheel.setTargetOutput(rightOutput);
    _leftWheel.setTargetOutput(leftOutput);
}

void DifferentialDrive::update() {
    _rightWheel.update();
    _leftWheel.update();
}

void DifferentialDrive::stop() {
    _rightWheel.stop();
    _leftWheel.stop();
}

void DifferentialDrive::brake() {
    _rightWheel.brake();
    _leftWheel.brake();
}
