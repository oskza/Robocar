#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <Arduino.h>
#include "../drive/DifferentialDrive.h"
#include "../odometry/Odometry.h"

class MotionController {
private:
    enum class State : uint8_t { IDLE, MANUAL, TIMED, DISTANCE };
    DifferentialDrive &_differential;
    Odometry &_odometry;
    State _state;
    uint32_t _endTime;
    uint32_t _targetTicks;
    void _idle();
public:
    MotionController(DifferentialDrive &differential, Odometry &odometry);
    void begin(uint8_t acceleration = 5);
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void driveDistance(int16_t velocity, float meters);
    void update(uint32_t nowMs);
    void stop();
    void brake();
};
#endif
