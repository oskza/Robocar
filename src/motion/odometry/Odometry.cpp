#include "Odometry.h"

Odometry::Odometry(Encoder &leftEncoder, Encoder &rightEncoder)
    : _leftEncoder(leftEncoder),
        _rightEncoder(rightEncoder),
        _wheelCircumference(0.0f),
        _encoderSlots(1) {}

void Odometry::begin(uint8_t encoderSlots, float wheelDiameterMeters, float wheelCircumferenceFactor) {
    _wheelCircumference = wheelDiameterMeters * PI * wheelCircumferenceFactor;
    _encoderSlots = max<uint8_t>(encoderSlots, 1);
    reset();
}

void Odometry::reset() {
    _leftEncoder.reset();
    _rightEncoder.reset();
}

uint32_t Odometry::getTicks() const {
    uint32_t left = _leftEncoder.getCount();
    uint32_t right = _rightEncoder.getCount();
    return (left >> 1) + (right >> 1) + ((left & 1) + (right & 1) + 1) / 2;
}

float Odometry::getMeters() const {
    float rotations = getTicks() / (float)_encoderSlots;
    return rotations * _wheelCircumference;
}

uint32_t Odometry::metersToTicks(float meters) const {
    if (meters <= 0.0f || _wheelCircumference <= 0.0f)
        return 0;

    return (uint32_t)round((meters / _wheelCircumference) * _encoderSlots);
}
