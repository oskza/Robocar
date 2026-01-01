#include "MotorController.h"

MotorController *MotorController::_instance = nullptr;

MotorController::MotorController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, 
                                    Timer &timer, StopWatch &stopwatch,
                                    double diameter, uint8_t slots) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _timer(timer), _stopwatch(stopwatch),
                                    _diameter(diameter), _slots(slots),
                                    _mode(DRIVE_MODE_AUTO), _targetTicks(0) { _instance = this; }

void IRAM_ATTR MotorController::_onRightEncoder() {
    if (_instance)
        _instance->_encoderRight.tick();
}
void IRAM_ATTR MotorController::_onLeftEncoder() {
    if (_instance)
        _instance->_encoderLeft.tick();
}

void MotorController::init(uint32_t freq, uint8_t res) {
    _motorRight.init(freq, res);
    _motorLeft.init(freq, res);
    _encoderRight.init(_onRightEncoder);
    _encoderLeft.init(_onLeftEncoder);
    _timer.reset();
    _stopwatch.stop();
    _mode = DRIVE_MODE_AUTO;
    _targetTicks = 0;
}

bool MotorController::tick() {
    if (!isDriving() || isModeManual())
        return false;
    if (_timer.isRunning() && _timer.tick()) {
        stop();
        _timer.reset();
        return true;
    }
    if (_targetTicks > 0 
            && _targetTicks <= getDistanceTicks()) {
        stop();
        _targetTicks = 0;
        return true;
    }
    return false;
}

void MotorController::stop() {
    if (!isDriving())
        return;
    _motorRight.stop(); 
    _motorLeft.stop();
    _encoderRight.reset();
    _encoderLeft.reset();
    _stopwatch.stop();
}

// velocity > 0 : forward
// velocity < 0 : backward
// turn     > 0 : rotate left (CCW)
// turn     < 0 : rotate right (CW)
void MotorController::driveDifferential(int16_t velocity, int16_t turn) {
    if (velocity == 0 && turn == 0) {
        stop();
        return;
    }
    int16_t pwmRight = velocity + turn;
    int16_t pwmLeft  = velocity - turn;
    int16_t maxMag = max(abs(pwmRight), abs(pwmLeft));
    if (maxMag > MOTOR_MAX_PWM) {
        pwmRight = pwmRight * MOTOR_MAX_PWM / maxMag;
        pwmLeft  = pwmLeft  * MOTOR_MAX_PWM / maxMag;
    }
    pwmRight = constrain(pwmRight, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    pwmLeft  = constrain(pwmLeft,  -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    _motorRight.setSignedPWM(pwmRight);
    _motorLeft.setSignedPWM(pwmLeft);
    _stopwatch.start();
}

void MotorController::driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left) {
    int16_t velocity = (up && !down)
                        ? velocityPWM
                        : (down && !up)
                            ? -(int16_t)velocityPWM
                            : 0;
    int16_t turn = (right && !left)
                    ? turnPWM
                    : (left && !right)
                        ? -(int16_t)turnPWM
                        : 0;
    driveDifferential(velocity, turn);
}

void MotorController::driveFor(int16_t velocity, int16_t turn, uint32_t ms) {
    if (ms == 0)
        return;
    _timer.setTimeout(ms);
    _timer.start();
    driveDifferential(velocity, turn);
}

void MotorController::driveDistance(int16_t velocity, float meters) {
    if (meters == 0)
        return;
    _encoderRight.reset();
    _encoderLeft.reset();
    _targetTicks = metersToTicks(meters, wheelCircumference(_diameter), _slots);
    driveDifferential(velocity, 0);
}

double MotorController::getDistanceTicks() const { 
    return (_encoderRight.getCount() + _encoderLeft.getCount()) / 2.00; 
}

double MotorController::getDistanceMeters() const { 
    float meters = ticksToMeters(getDistanceTicks(), wheelCircumference(_diameter), _slots);
    return meters; 
}

uint32_t MotorController::getDurationMs() const { return _stopwatch.lap(); }

void MotorController::setMode(const char *mode) {
    if (!mode) 
        return;
    if(strcmp(mode, "auto") == 0) {
        setModeAuto();
        return;
    }
    if (strcmp(mode, "manual") == 0)
        setModeManual();
}

void MotorController::setModeAuto() { _mode = DRIVE_MODE_AUTO; }

void MotorController::setModeManual() { _mode = DRIVE_MODE_MANUAL; }

bool MotorController::isModeAuto() const { return _mode == DRIVE_MODE_AUTO; }

bool MotorController::isModeManual() const { return _mode == DRIVE_MODE_MANUAL; }

bool MotorController::isDriving() const { return _motorRight.getPWM() > 0 || _motorLeft.getPWM() > 0; }

void MotorController::getStatus(JsonObject &target) const {
    bool driving = isDriving();
    target["mode"] = (isModeAuto()) ? "auto" : "manual";
    target["driving"] = driving;
    if (driving) {
        target["pwmRight"] = _motorRight.getPWM();
        target["pwmLeft"] = _motorLeft.getPWM();
        target["distance"] = getDistanceMeters();
        target["duration"] = getDurationMs();
    }
}
