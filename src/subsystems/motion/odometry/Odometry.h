#ifndef ODOMETRY_H
#define ODOMETRY_H
#include <Arduino.h>
#include <Encoder.h>

class Odometry {
private:
    Encoder &_leftEncoder;
    Encoder &_rightEncoder;
    float _wheelCircumference;
    uint8_t _encoderSlots;
public:
    Odometry(Encoder &leftEncoder, Encoder &rightEncoder);
    void begin(uint8_t encoderSlots, float wheelDiameterMeters, float wheelCircumferenceFactor = 1.0f);
    void reset();
    uint32_t getTicks() const;
    float getMeters() const;
    uint32_t metersToTicks(float meters) const;
};
#endif
