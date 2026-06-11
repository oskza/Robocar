#include "DriveController.h"

DriveController *DriveController::_instance = nullptr;

DriveController::DriveController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, 
                                    Timer &timer, StopWatch &stopwatch) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _timer(timer), _stopwatch(stopwatch),
                                    _mode(DriveMode::AUTO), _circumference(0), _encoderSlots(20),
                                    _targetTicks(0) { _instance = this; }

void IRAM_ATTR DriveController::_onRightEncoder() { if (_instance) _instance->_encoderRight.tick(); }

void IRAM_ATTR DriveController::_onLeftEncoder() { if (_instance) _instance->_encoderLeft.tick(); }

void DriveController::init(const DriveConfig &cfg, DriveMode mode) {
    _motorRight.init(cfg.pwmFrequency, cfg.pwmResolution, cfg.motorRightMinPWM);
    _motorLeft.init(cfg.pwmFrequency, cfg.pwmResolution, cfg.motorLeftMinPWM);
    _encoderRight.init(_onRightEncoder);
    _encoderLeft.init(_onLeftEncoder);
    _timer.reset();
    _stopwatch.stop();
    _mode = mode;
    _circumference = wheelCircumference(cfg.wheelDiameter, cfg.wheelCircumferenceFactor);
    _encoderSlots = cfg.encoderSlots;
    _targetTicks = 0;
}

void DriveController::stop() {
    if (!isDriving())
        return;
    _motorRight.stop(); 
    _motorLeft.stop();
    _encoderRight.reset();
    _encoderLeft.reset();
    _stopwatch.stop();
    if (isModeAuto()) {
        _timer.reset();
        _targetTicks = 0;
    }
}

bool DriveController::tick() {
    if (isModeAuto() && isDriving() 
            && (_timer.tick() 
                || (_targetTicks > 0 && getDistanceTicks() >= _targetTicks))) {
        stop();
        return true;
    }
    return false;
}

/**
 * velocity > 0 : forward 
 * velocity < 0 : backward 
 * turn     > 0 : rotate left (CCW) 
 * turn     < 0 : rotate right (CW) 
*/
void DriveController::driveDifferential(int16_t velocity, int16_t turn) {
    if (velocity == 0 && turn == 0) {
        stop();
        return;
    }
    int16_t pwmRight = velocity + turn;
    int16_t pwmLeft  = velocity - turn;
    int16_t maxMag = max(abs(pwmRight), abs(pwmLeft));
    if (maxMag > 255) {
        pwmRight = pwmRight * 255 / maxMag;
        pwmLeft  = pwmLeft  * 255 / maxMag;
    }
    _motorRight.run(pwmRight);
    _motorLeft.run(pwmLeft);
    _stopwatch.start();
}

void DriveController::driveDiscreteArcade(uint8_t velocityPwm, uint8_t turnPwm, bool up, bool down, bool right, bool left) {
    int16_t velocity = (up && !down)
                        ? velocityPwm
                        : (down && !up)
                            ? -(int16_t)velocityPwm
                            : 0;
    int16_t turn = (right && !left)
                    ? turnPwm
                    : (left && !right)
                        ? -(int16_t)turnPwm
                        : 0;
    driveDifferential(velocity, turn);
}

void DriveController::driveFor(int16_t velocity, int16_t turn, uint32_t ms) {
    if (ms == 0)
        return;
    _timer.setTimeout(ms);
    _timer.start();
    driveDifferential(velocity, turn);
}

void DriveController::driveDistance(int16_t velocity, float meters) {
    if (meters <= 0)
        return;
    _encoderRight.reset();
    _encoderLeft.reset();
    _targetTicks = metersToTicks(meters, _circumference, _encoderSlots);
    driveDifferential(velocity, 0);
}

void DriveController::setModeAuto() { _mode = DriveMode::AUTO; }

void DriveController::setModeManual() { _mode = DriveMode::MANUAL; }

bool DriveController::isModeAuto() const { return _mode == DriveMode::AUTO; }

bool DriveController::isModeManual() const { return _mode == DriveMode::MANUAL; }

bool DriveController::isDriving() const { return _motorRight.getPwm() > 0 || _motorLeft.getPwm() > 0; }

uint32_t DriveController::getDistanceTicks() const { 
    uint32_t a = _encoderRight.getCount();
    uint32_t b = _encoderLeft.getCount();
    return (a >> 1) + (b >> 1) + ((a & 1) + (b & 1) + 1) / 2;
}

float DriveController::getDistanceMeters() const { return ticksToMeters(getDistanceTicks(), _circumference, _encoderSlots); }

uint32_t DriveController::getDurationMs() const { return _stopwatch.lap(); }
