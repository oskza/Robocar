#include "MotionController.h"
#include <AngleMath.h>

MotionController::MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass)
    : _differential(differential),
      _odometry(odometry),
      _compass(compass),
      _cfg{},
      _state(MotionState::IDLE) {}

void MotionController::_clearTargets() {
    _timed = {};
    _distance = {};
    _rotation = {};
}

void MotionController::_updateRotation() {
    if (!_compass.isAvailable()) {
        stop();
        return;
    }
    const float diff = AngleMath::differenceDegrees(_compass.getHeadingDegrees(), _rotation.headingDegrees);
    if (fabsf(diff) <= _cfg.headingToleranceDegrees) {
        brake();
        return;
    }
    int16_t turn = (diff > 0.0f) ? -(int16_t)_rotation.speed :  (int16_t)_rotation.speed;
    _differential.drive(0, turn);
}

void MotionController::begin(const MotionConfig &cfg, uint32_t pwmFrequency, uint8_t encoderSlots) {
    _cfg = cfg;

    _differential.begin(
        _cfg.wheelAcceleration,
        pwmFrequency,
        _cfg.motorLeftMinPwm,
        _cfg.motorRightMinPwm
    );

    _odometry.begin(
        encoderSlots,
        _cfg.wheelDiameterMeters,
        _cfg.wheelCircumferenceFactor
    );

    if (!_compass.begin()) {}

    _clearTargets();
    _state = MotionState::IDLE;
}

void MotionController::drive(int16_t velocity, int16_t turn) {
    _clearTargets();
    _state = MotionState::MANUAL;
    _differential.drive(velocity, turn);
}

void MotionController::driveFor(int16_t velocity, int16_t turn, uint32_t durationMs) {
    if (durationMs == 0) {
        stop();
        return;
    }
    _clearTargets();
    _state = MotionState::TIMED;
    _timed.endTimeMs = millis() + durationMs;
    _differential.drive(velocity, turn);
}

void MotionController::driveDistance(int16_t velocity, float meters) {
    if (meters <= 0.0f || velocity == 0) {
        stop();
        return;
    }
    const uint32_t targetTicks = _odometry.metersToTicks(meters);
    if (targetTicks == 0) {
        stop();
        return;
    }
    _clearTargets();
    _state = MotionState::DISTANCE;
    _distance.startTicks = _odometry.getTicks();
    _distance.targetTicks = targetTicks;
    _differential.drive(velocity, 0);
}

void MotionController::rotateTo(float headingDegrees, uint8_t speed) {
    if (!_compass.isAvailable() || speed == 0) {
        stop();
        return;
    }
    _clearTargets();
    _state = MotionState::ROTATING;
    _rotation.headingDegrees = AngleMath::normalizeDegrees(headingDegrees);
    _rotation.speed = (speed > MotorDriver::MAX_OUTPUT) ? MotorDriver::MAX_OUTPUT : speed;
}

void MotionController::rotateBy(float degrees, uint8_t speed) {
    rotateTo(_compass.getHeadingDegrees() + degrees, speed);
}

void MotionController::update(uint32_t nowMs) {
    switch (_state) {
        case MotionState::TIMED:
            if (_timed.expired(nowMs))
                brake();
            break;
        case MotionState::DISTANCE:
            if (_distance.reached(_odometry.getTicks()))
                brake();
            break;
        case MotionState::ROTATING:
            _updateRotation();
            break;
        case MotionState::IDLE:
        case MotionState::MANUAL:
            break;
    }
    _differential.update();
}

void MotionController::stop() {
    _clearTargets();
    _state = MotionState::IDLE;
    _differential.stop();
}

void MotionController::brake() {
    _clearTargets();
    _state = MotionState::IDLE;
    _differential.brake();
}

bool MotionController::isStopped() const { return _state == MotionState::IDLE && _differential.isStopped(); }

MotionState MotionController::getState() const { return _state; }

MotionSnapshot MotionController::getSnapshot() const {
    MotionSnapshot snapshot{};
    snapshot.state = _state;
    snapshot.stopped = isStopped();
    snapshot.heading.currentDegrees = _compass.getHeadingDegrees();
    snapshot.heading.targetDegrees = _rotation.headingDegrees;
    snapshot.heading.errorDegrees = (_state == MotionState::ROTATING)
        ? AngleMath::differenceDegrees(snapshot.heading.targetDegrees, snapshot.heading.currentDegrees)
        : 0.0f;
    snapshot.output.left = _differential.getLeftOutput();
    snapshot.output.right = _differential.getRightOutput();
    snapshot.odometry.distanceMeters = _odometry.getMeters();
    snapshot.odometry.averageTicks = _odometry.getTicks();
    return snapshot;
}

void MotionController::getConfig(MotionConfig &cfg) const { cfg = _cfg; }

void MotionController::setConfig(const MotionConfig &cfg) { _cfg = cfg; }
