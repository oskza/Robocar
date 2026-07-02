#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Arduino.h>
#include <AngleMath.h>
#include <Bmm150Compass.h>
#include "../drive/DifferentialDrive.h"
#include "../odometry/Odometry.h"
#include "MotionState.h"
#include "MotionSnapshot.h"
#include "MotionCommand.h"

class MotionController {
private:
    DifferentialDrive &_differential;
    Odometry &_odometry;
    Bmm150Compass &_compass;
    MotionState _state;
    float _headingToleranceDegrees;
    struct TimedTarget {
        uint32_t endTimeMs = 0;
        bool expired(uint32_t nowMs) const { return (int32_t)(nowMs - endTimeMs) >= 0; }
    } _timed;
    struct DistanceTarget {
        uint32_t targetTicks = 0;
        bool reached(uint32_t ticks) const { return ticks >= targetTicks; }
    } _distance;
    struct RotationTarget {
        float headingDegrees = 0.0f;
        uint8_t speed = 0;
    } _rotation;
    void _clearTargets();
    void _updateRotation();
public:
    MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass);
    void begin(uint8_t acceleration = 5, float headingToleranceDegrees = 2.0f);
    MotionState getState() const;
    MotionSnapshot getSnapshot() const;
    void execute(const MotionCommand &cmd);
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void rotateTo(float headingDegrees, uint8_t speed);
    void rotateBy(float degrees, uint8_t speed);
    void update(uint32_t nowMs);
    void stop();
    void brake();
    bool isStopped() const;
};
#endif
