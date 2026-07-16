#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Bmm150Compass.h>
#include "drive/DifferentialDrive.h"
#include "odometry/Odometry.h"
#include "MotionState.h"
#include "MotionConfig.h"
#include "MotionSnapshot.h"

class MotionController {
private:
    DifferentialDrive &_differential;
    Odometry &_odometry;
    Bmm150Compass &_compass;
    MotionConfig _cfg;
    MotionState _state;
    struct TimedTarget {
        uint32_t durationMs = 0;
        uint32_t endTimeMs = 0;
        int16_t velocity = 0;
        int16_t turn = 0;
        bool started = false;
        bool expired(uint32_t nowMs) const {
            return started && (int32_t)(nowMs - endTimeMs) >= 0;
        }
    } _timed;
    struct DistanceTarget {
        uint32_t startTicks = 0;
        uint32_t targetTicks = 0;
        bool reached(uint32_t currentTicks) const { return currentTicks - startTicks >= targetTicks; }
    } _distance;
    struct RotationTarget {
        float headingDegrees = 0.0f;
        uint8_t speed = 0;
    } _rotation;
    void _clearTargets();
    void _updateRotation();
public:
    MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass);
    void begin(const MotionConfig &cfg, uint32_t pwmFrequency, uint8_t encoderSlots);
    void update(uint32_t nowMs);
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void rotateTo(float headingDegrees, uint8_t speed);
    void rotateBy(float degrees, uint8_t speed);
    void stop();
    void brake();
    void resetOdometry();
    bool isStopped() const;
    MotionState getState() const;
    MotionSnapshot getSnapshot() const;
    void getConfig(MotionConfig &cfg) const;
    void setConfig(const MotionConfig &cfg);
};
#endif
