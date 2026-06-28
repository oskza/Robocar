#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include <Arduino.h>
#include "../drive/DifferentialDrive.h"

class MotionController {
private:
    enum class State : uint8_t { IDLE, MANUAL, TIMED };
    DifferentialDrive &_differential;
    State _state;
    uint32_t _endTime;
    void _idle();
public:
    MotionController(DifferentialDrive &differential);
    void begin(uint8_t acceleration = 5);
    void drive(int16_t velocity, int16_t turn);
    void driveFor(int16_t velocity, int16_t turn, uint32_t durationMs);
    void update(uint32_t nowMs);
    void stop();
    void brake();
};
#endif
