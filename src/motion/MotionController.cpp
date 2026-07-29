#include "MotionController.h"
#include <AngleMath.h>
#include <math.h>

MotionController::MotionController(DifferentialDrive &differential, Odometry &odometry, Bmm150Compass &compass)
    : _differential(differential),
      _odometry(odometry),
      _compass(compass),
      _cfg{},
      _state(MotionState::IDLE),
      _timed{},
      _distance{},
      _rotation{} {}

void MotionController::_clearTargets() {
    _timed = {};
    _distance = {};
    _rotation = {};
}

void MotionController::_updateTimed(uint32_t nowMs) {
    if (!_timed.timer.isRunning()) {
        _timed.timer.start(nowMs);
        _differential.drive(_timed.velocity, _timed.turn);
        return;
    }
    if (_timed.timer.hasElapsed(nowMs, _timed.durationMs))
        brake();
}

void MotionController::_updateRotation() {
    float currentHeading = 0.0f;
    if (!_compass.readHeadingDegrees(currentHeading)) {
        stop();
        return;
    }
    const float diff = AngleMath::differenceDegrees(currentHeading, _rotation.headingDegrees);
    if (fabsf(diff) <= _cfg.headingToleranceDegrees) {
        brake();
        return;
    }
    const int16_t turn = (diff > 0.0f) ? -(int16_t)_rotation.speed : (int16_t)_rotation.speed;
    _differential.drive(0, turn);
}

void MotionController::begin(const MotionConfig &cfg, uint32_t pwmFrequency, uint8_t encoderSlots) {
    _cfg = cfg;
    _differential.begin(_cfg.wheelAcceleration, pwmFrequency, _cfg.motorLeftMinPwm, _cfg.motorRightMinPwm);
    _odometry.begin(encoderSlots, _cfg.wheelDiameterMeters, _cfg.wheelCircumferenceFactor);
    _compass.begin();
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
    _timed.durationMs = durationMs;
    _timed.velocity = velocity;
    _timed.turn = turn;
    _state = MotionState::TIMED;
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
    _distance.startTicks = _odometry.getTicks();
    _distance.targetTicks = targetTicks;
    _state = MotionState::DISTANCE;
    _differential.drive(velocity, 0);
}

void MotionController::rotateTo(float headingDegrees, uint8_t speed) {
    if (!_compass.isAvailable() || speed == 0) {
        stop();
        return;
    }
    _clearTargets();
    _rotation.headingDegrees = AngleMath::normalizeDegrees(headingDegrees);
    _rotation.speed = speed;
    _state = MotionState::ROTATING;
}

void MotionController::rotateBy(float degrees, uint8_t speed) {
    float currentHeading = 0.0f;
    if (!_compass.readHeadingDegrees(currentHeading)) {
        stop();
        return;
    }
    rotateTo(currentHeading + degrees, speed);
}

void MotionController::update(uint32_t nowMs) {
    switch (_state) {
        case MotionState::TIMED:
            _updateTimed(nowMs);
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

void MotionController::resetOdometry() { _odometry.reset(); }

bool MotionController::isStopped() const { return _state == MotionState::IDLE && _differential.isStopped(); }

MotionState MotionController::getState() const { return _state; }

MotionSnapshot MotionController::getSnapshot() {
    MotionSnapshot snapshot{};
    snapshot.state = _state;
    snapshot.stopped = isStopped();
    snapshot.output.left = _differential.getLeftOutput();
    snapshot.output.right = _differential.getRightOutput();
    snapshot.heading.available = _compass.isAvailable();
    if (snapshot.heading.available) {
        _compass.readHeadingDegrees(snapshot.heading.currentDegrees);
        if (snapshot.state == MotionState::ROTATING) {
            snapshot.heading.targetDegrees = _rotation.headingDegrees;
            snapshot.heading.errorDegrees = AngleMath::differenceDegrees(
                snapshot.heading.targetDegrees, snapshot.heading.currentDegrees
            );
        }
    }
    snapshot.odometry.distanceMeters = _odometry.getMeters();
    snapshot.odometry.averageTicks = _odometry.getTicks();
    return snapshot;
}

void MotionController::getConfig(MotionConfig &cfg) const { cfg = _cfg; }

void MotionController::setConfig(const MotionConfig &cfg) {
    if (!isStopped())
        brake();
    _cfg = cfg;
    _differential.setConfig(_cfg.wheelAcceleration, _cfg.motorLeftMinPwm, _cfg.motorRightMinPwm);
    _odometry.setWheelGeometry(_cfg.wheelDiameterMeters, _cfg.wheelCircumferenceFactor);
}
