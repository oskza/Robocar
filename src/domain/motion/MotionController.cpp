#include "MotionController.h"

MotionController::MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass)
    : _differential(differential),
      _odometry(odometry),
      _compass(compass),
      _state(State::IDLE),
      _endTime(0),
      _targetTicks(0),
      _targetHeadingDegrees(0.0f),
      _rotationSpeed(0),
      _headingToleranceDegrees(2.0f) {}

void MotionController::_idle() {
    _state = State::IDLE;
    _endTime = 0;
    _targetTicks = 0;
    _targetHeadingDegrees = 0.0f;
    _rotationSpeed = 0;
}

void MotionController::begin(uint8_t acceleration, float headingToleranceDegrees) {
    _differential.begin(acceleration);
    _headingToleranceDegrees = headingToleranceDegrees;
    _idle();
}

void MotionController::drive(int16_t velocity, int16_t turn) {
    _state = State::MANUAL;
    _endTime = 0;
    _targetTicks = 0;
    _targetHeadingDegrees = 0.0f;
    _rotationSpeed = 0;
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
    _targetHeadingDegrees = 0.0f;
    _rotationSpeed = 0;
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
    _targetHeadingDegrees = 0.0f;
    _rotationSpeed = 0;
    _differential.drive(velocity, 0);
}

void MotionController::rotateTo(float headingDegrees, uint8_t speed) {
    if (speed == 0) {
        stop();
        return;
    }
    _state = State::ROTATING;
    _endTime = 0;
    _targetTicks = 0;
    _targetHeadingDegrees = AngleMath::normalizeDegrees(headingDegrees);
    _rotationSpeed = constrain(speed, 1, MotorDriver::MAX_OUTPUT);
}

void MotionController::rotateBy(float degrees, uint8_t speed) {
    rotateTo(_compass.getHeadingDegrees() + degrees, speed);
}

void MotionController::update(uint32_t nowMs) {
    if ((_state == State::TIMED && (int32_t)(nowMs - _endTime) >= 0)
            || (_state == State::DISTANCE && _odometry.getTicks() >= _targetTicks)) {
        stop();
    } else if (_state == State::ROTATING) {
        float diff = AngleMath::differenceDegrees(_compass.getHeadingDegrees(), _targetHeadingDegrees);
        if (fabsf(diff) <= _headingToleranceDegrees) {
            stop();
        } else {
            _differential.drive(
                0,
                diff > 0.0f
                    ? (int16_t)_rotationSpeed
                    : -(int16_t)_rotationSpeed);
        }
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
