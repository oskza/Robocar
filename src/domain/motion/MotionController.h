#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Arduino.h>
#include <AngleMath.h>
#include <Bmm150Compass.h>
#include "../drive/DifferentialDrive.h"
#include "../odometry/Odometry.h"

class MotionController {
private:
    enum class State : uint8_t { IDLE, MANUAL, TIMED, DISTANCE, ROTATING };
    DifferentialDrive &_differential;
    Odometry &_odometry;
    Bmm150Compass &_compass;
    State _state;
    uint32_t _endTime;
    uint32_t _targetTicks;
    float _targetHeadingDegrees;
    uint8_t _rotationSpeed;
    float _headingToleranceDegrees;
    void _idle();
public:
    MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass);
    void begin(uint8_t acceleration = 5, float headingToleranceDegrees = 2.0f);
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void rotateTo(float headingDegrees, uint8_t speed);
    void rotateBy(float degrees, uint8_t speed);
    void update(uint32_t nowMs);
    void stop();
    void brake();
};
#endif
