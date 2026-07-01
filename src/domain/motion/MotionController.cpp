#include "MotionController.h"

MotionController::MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass)
    : _differential(differential),
      _odometry(odometry),
      _compass(compass),
      _state(State::IDLE),
      _timed{0},
      _distance{0},
      _rotation{0.0f, 0},
      _headingToleranceDegrees(2.0f) {}

void MotionController::_clearCommands() {
    _timed.clear();
    _distance.clear();
    _rotation.clear();
}

bool MotionController::_timedExpired(uint32_t nowMs) const {
    return (int32_t)(nowMs - _timed.endTime) >= 0;
}

bool MotionController::_distanceReached() const {
    return _odometry.getTicks() >= _distance.targetTicks;
}

void MotionController::_updateRotation() {
    float diff = AngleMath::differenceDegrees(_compass.getHeadingDegrees(), _rotation.targetHeadingDegrees);
    if (fabsf(diff) <= _headingToleranceDegrees) {
        brake();
        return;
    }
    int16_t turn = (diff > 0.0f) ? -(int16_t)_rotation.speed :  (int16_t)_rotation.speed;
    _differential.drive(0, turn);
}

void MotionController::begin(uint8_t acceleration, float headingToleranceDegrees) {
    _differential.begin(acceleration);
    _headingToleranceDegrees = headingToleranceDegrees;
    _clearCommands();
    _state = State::IDLE;
}

void MotionController::drive(int16_t velocity, int16_t turn) {
    _clearCommands();
    _state = State::MANUAL;
    _differential.drive(velocity, turn);
}

void MotionController::driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
    if (durationMs == 0) {
        stop();
        return;
    }
    _clearCommands();
    _state = State::TIMED;
    _timed.endTime = millis() + durationMs;
    _differential.drive(velocity, turn);
}

void MotionController::driveDistance(int16_t velocity, float meters) {
    if (meters <= 0.0f || velocity == 0) {
        stop();
        return;
    }
    uint32_t targetTicks = _odometry.metersToTicks(meters);
    if (targetTicks == 0) {
        stop();
        return;
    }
    _odometry.reset();
    _clearCommands();
    _state = State::DISTANCE;
    _distance.targetTicks = targetTicks;
    _differential.drive(velocity, 0);
}

void MotionController::rotateTo(float headingDegrees, uint8_t speed) {
    if (speed == 0) {
        stop();
        return;
    }
    _clearCommands();
    _state = State::ROTATING;
    _rotation.targetHeadingDegrees = AngleMath::normalizeDegrees(headingDegrees);
    _rotation.speed = constrain(speed, 1, MotorDriver::MAX_OUTPUT);
}

void MotionController::rotateBy(float degrees, uint8_t speed) {
    rotateTo(_compass.getHeadingDegrees() + degrees, speed);
}

void MotionController::update(uint32_t nowMs) {
    switch (_state) {
        case State::TIMED:
            if (_timedExpired(nowMs))
                stop();
            break;
        case State::DISTANCE:
            if (_distanceReached())
                stop();
            break;
        case State::ROTATING:
            _updateRotation();
            break;
        case State::IDLE:
        case State::MANUAL:
            break;
    }
    _differential.update();
}

void MotionController::stop() {
    _clearCommands();
    _state = State::IDLE;
    _differential.stop();
}

void MotionController::brake() {
    _clearCommands();
    _state = State::IDLE;
    _differential.brake();
}

bool MotionController::isStopped() const { return _state == State::IDLE && _differential.isStopped(); }
