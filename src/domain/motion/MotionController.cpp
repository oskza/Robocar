#include "MotionController.h"

MotionController::MotionController(DifferentialDrive &differential, Odometry &odometry)
    : _differential(differential),
      _odometry(odometry),
      _state(State::IDLE),
      _endTime(0),
      _targetTicks(0) {}

void MotionController::_idle() {
    _state = State::IDLE;
    _endTime = 0;
    _targetTicks = 0;
}

void MotionController::begin(uint8_t acceleration) {
    _differential.begin(acceleration);
    _idle();
}

void MotionController::drive(int16_t velocity, int16_t turn) {
    _state = State::MANUAL;
    _endTime = 0;
    _targetTicks = 0;
    _differential.drive(velocity, turn);
}

void MotionController::driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
    if (durationMs == 0) {
        stop();
        return;
    }
    _state = State::TIMED;
    _endTime = millis() + durationMs;
    _targetTicks = 0;
    _differential.drive(velocity, turn);
}

void MotionController::driveDistance(int16_t velocity, float meters) {
    if (meters <= 0.0f || velocity == 0) {
        stop();
        return;
    }
    _targetTicks = _odometry.metersToTicks(meters);
    if (_targetTicks == 0) {
        stop();
        return;
    }
    _odometry.reset();
    _state = State::DISTANCE;
    _endTime = 0;
    _differential.drive(velocity, 0);
}

void MotionController::update(uint32_t nowMs) {
    if ((_state == State::TIMED && (int32_t)(nowMs - _endTime) >= 0)
            || (_state == State::DISTANCE && _odometry.getTicks() >= _targetTicks)) {
        stop();
    }
    _differential.update();
}

void MotionController::stop() {
    _idle();
    _differential.stop();
}

void MotionController::brake() {
    _idle();
    _differential.brake();
}
