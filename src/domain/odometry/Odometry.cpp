#include "Odometry.h"

Odometry::Odometry(Encoder &rightEncoder, Encoder &leftEncoder)
    : _rightEncoder(rightEncoder),
      _leftEncoder(leftEncoder),
      _wheelCircumference(0.0f),
      _encoderSlots(1) {}

void Odometry::begin(float wheelCircumference, uint8_t encoderSlots) {
    _wheelCircumference = wheelCircumference;
    _encoderSlots = max<uint8_t>(encoderSlots, 1);
    reset();
}

void Odometry::reset() {
    _rightEncoder.reset();
    _leftEncoder.reset();
}

uint32_t Odometry::getTicks() const {
    uint32_t right = _rightEncoder.getCount();
    uint32_t left = _leftEncoder.getCount();
    return (right >> 1) + (left >> 1) + ((right & 1) + (left & 1) + 1) / 2;
}

float Odometry::getMeters() const {
    float rotations = getTicks() / (float)_encoderSlots;
    return rotations * _wheelCircumference;
}
