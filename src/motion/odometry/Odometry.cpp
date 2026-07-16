#include "Odometry.h"

Odometry::Odometry(Encoder &leftEncoder, Encoder &rightEncoder)
    : _leftEncoder(leftEncoder),
      _rightEncoder(rightEncoder),
      _wheelCircumference(0.0f),
      _encoderSlots(1) {}

void Odometry::begin(
    uint8_t encoderSlots,
    float wheelDiameterMeters,
    float wheelCircumferenceFactor
) {
    _wheelCircumference = wheelDiameterMeters * PI * wheelCircumferenceFactor;
    _encoderSlots = max<uint8_t>(encoderSlots, 1);
    reset();
}

void Odometry::reset() {
    _leftEncoder.reset();
    _rightEncoder.reset();
}

uint32_t Odometry::getTicks() const {
    const uint32_t leftTicks = _leftEncoder.getCount();
    const uint32_t rightTicks = _rightEncoder.getCount();
    return (leftTicks >> 1) + (rightTicks >> 1) + ((leftTicks & 1U) + (rightTicks & 1U) + 1U) / 2U;
}

float Odometry::getMeters() const {
    const float rotations = getTicks() / (float)_encoderSlots;
    return rotations * _wheelCircumference;
}

uint32_t Odometry::metersToTicks(float meters) const {
    return (meters > 0.0f && _wheelCircumference > 0.0f)
        ? (uint32_t)round((meters / _wheelCircumference) * _encoderSlots)
        : 0;
}
