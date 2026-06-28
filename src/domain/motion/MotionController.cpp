#include "MotionController.h"

MotionController::MotionController(DifferentialDrive &differential)
    : _differential(differential),
      _state(State::IDLE),
      _endTime(0) {}

void MotionController::_idle() {
    _state = State::IDLE;
    _endTime = 0;
}

void MotionController::begin(uint8_t acceleration) {
    _differential.begin(acceleration);
    _idle();
}

void MotionController::drive(int16_t velocity, int16_t turn) {
    _state = State::MANUAL;
    _differential.drive(velocity, turn);
}

void MotionController::driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
    if (durationMs == 0) {
        stop();
        return;
    }
    _state = State::TIMED;
    _endTime = millis() + durationMs;
    _differential.drive(velocity, turn);
}

void MotionController::update(uint32_t nowMs) {
    if (_state == State::TIMED && (int32_t)(nowMs - _endTime) >= 0)
        stop();
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
