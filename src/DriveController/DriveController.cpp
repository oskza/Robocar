#include "DriveController.h"

DriveController::DriveController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, 
                                    Timer &timer, StopWatch &stopwatch) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _timer(timer), _stopwatch(stopwatch),
                                    _mode(DRIVE_MODE_AUTO), _targetTicks(0) {}

void DriveController::init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res) {
    _motorRight.init(freq, res);
    _motorLeft.init(freq, res);
    _encoderRight.init(onRightEncoder);
    _encoderLeft.init(onLeftEncoder);
    _timer.reset();
    _stopwatch.stop();
    _mode = DRIVE_MODE_AUTO;
    _targetTicks = 0;
}

void DriveController::tick() {
    if (!isDriving() || isModeManual())
        return;
    if (_timer.isRunning() && _timer.tick()) {
        stop();
        _timer.reset();
        return;
    }
    if (_targetTicks > 0 
            && _targetTicks <= getDistanceTicks()) {
        stop();
        _targetTicks = 0;
    }
}

void DriveController::stop() {
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
void DriveController::driveDifferential(int16_t velocity, int16_t turn) {
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

void DriveController::driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left) {
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

void DriveController::driveFor(int16_t velocity, int16_t turn, uint32_t ms) {
    _timer.setTimeout(ms);
    _timer.start();
    driveDifferential(velocity, turn);
}

void DriveController::driveDistance(int16_t velocity, float meters) {
    if (meters == 0)
        return;
    _encoderRight.reset();
    _encoderLeft.reset();
    _targetTicks = METERS_TO_TICKS(meters);
    driveDifferential(velocity, 0);
}

float DriveController::getDistanceTicks() const { 
    return (_encoderRight.getCount() + _encoderLeft.getCount()) / 2.00; 
}

float DriveController::getDistanceMeters() const { 
    float meters = TICKS_TO_METERS(getDistanceTicks());
    Serial.printf("meters: %.4f", meters);
    return meters; 
}

uint32_t DriveController::getDurationMs() const { return _stopwatch.lap(); }

void DriveController::setMode(const char *mode) {
    if (!mode) 
        return;
    if(strcmp(mode, "auto") == 0) {
        setModeAuto();
        return;
    }
    if (strcmp(mode, "manual") == 0)
        setModeManual();
}

void DriveController::setModeAuto() { _mode = DRIVE_MODE_AUTO; }

void DriveController::setModeManual() { _mode = DRIVE_MODE_MANUAL; }

bool DriveController::isModeAuto() const { return _mode == DRIVE_MODE_AUTO; }

bool DriveController::isModeManual() const { return _mode == DRIVE_MODE_MANUAL; }

bool DriveController::isDriving() const { return _motorRight.getPWM() > 0 || _motorLeft.getPWM() > 0; }

void DriveController::getStatus(JsonObject &target) const {
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
