#ifndef ODOMETRY_H
#define ODOMETRY_H
#include <Arduino.h>
#include <Encoder.h>

class Odometry {
private:
    Encoder &_rightEncoder;
    Encoder &_leftEncoder;
    float _wheelCircumference;
    uint8_t _encoderSlots;
public:
    Odometry(Encoder &rightEncoder, Encoder &leftEncoder);
    void begin(float wheelCircumference, uint8_t encoderSlots);
    void reset();
    uint32_t getTicks() const;
    float getMeters() const;
};
#endif
