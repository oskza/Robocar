#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Arduino.h>
#include <AngleMath.h>
#include <Bmm150Compass.h>
#include "../drive/DifferentialDrive.h"
#include "../odometry/Odometry.h"
#include "MotionState.h"
#include "MotionSnapshot.h"

class MotionController {
private:
    struct TimedCommand {
        uint32_t endTime;
        void clear() { endTime = 0; }
    };
    struct DistanceCommand {
        uint32_t targetTicks;
        void clear() { targetTicks = 0; }
    };
    struct RotationCommand {
        float targetHeadingDegrees;
        uint8_t speed;
        void clear() {
            targetHeadingDegrees = 0.0f;
            speed = 0;
        }
    };
    DifferentialDrive &_differential;
    Odometry &_odometry;
    Bmm150Compass &_compass;
    MotionState _state;
    TimedCommand _timed;
    DistanceCommand _distance;
    RotationCommand _rotation;
    float _headingToleranceDegrees;
    void _clearCommands();
    bool _timedExpired(uint32_t nowMs) const;
    bool _distanceReached() const;
    void _updateRotation();
public:
    MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass);
    void begin(uint8_t acceleration = 5, float headingToleranceDegrees = 2.0f);
    MotionState getState() const;
    MotionSnapshot getSnapshot() const;
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
