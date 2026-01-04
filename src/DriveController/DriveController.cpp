#include "DriveController.h"

DriveController *DriveController::_instance = nullptr;

DriveController::DriveController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, 
                                    CompassBMM150 &compass, Timer &timer, StopWatch &stopwatch, 
                                    uint8_t slots, double diameter) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _compass(compass), _timer(timer), _stopwatch(stopwatch), 
                                    _mode(DRIVE_MODE_MANUAL), _autoState(AUTO_DRIVE_STATE_IDLE),
                                    _slots(slots), _targetTicks(0), _startHeading(0), _targetHeading(0) { 
    _circumference = wheelCircumference(diameter);
    _instance = this;
}

void IRAM_ATTR DriveController::_onRightEncoder() {
    if (_instance)
        _instance->_encoderRight.tick();
}

void IRAM_ATTR DriveController::_onLeftEncoder() {
    if (_instance)
        _instance->_encoderLeft.tick();
}

void DriveController::init(uint32_t freq, uint8_t res) {
    _motorRight.init(freq, res);
    _motorLeft.init(freq, res);
    _encoderRight.init(_onRightEncoder);
    _encoderLeft.init(_onLeftEncoder);
    if (!_compass.init()) {/*...*/}
    _timer.reset();
    _stopwatch.stop();
    _mode = DRIVE_MODE_MANUAL;
    _autoState = AUTO_DRIVE_STATE_IDLE;
    _targetTicks = 0;
    _startHeading = 0;
    _targetHeading = 0;
}

/* TODO */
bool DriveController::tick() {
    if (isModeManual() || !isDriving())
        return false;
    switch (_autoState) {
        case AUTO_DRIVE_STATE_TIME:
            if (_timer.tick()) {
                stop();
                return true;
            }
            break;
        case AUTO_DRIVE_STATE_DISTANCE:
            if (getDistanceTicks() >= _targetTicks) {
                stop();
                return true;
            }
            break;
        case AUTO_DRIVE_STATE_ROTATION:
            // double curr = _compass.getCompassDegree();
            // if (true) {
            //     stop();
            //     return true;
            // }
            break;
        case AUTO_DRIVE_STATE_IDLE:
        default:
            break;
    }
    return false;
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
        _startHeading = 0;
        _targetHeading = 0;
        _autoState = AUTO_DRIVE_STATE_IDLE;
    }
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
    if (ms == 0)
        return;
    _autoState = AUTO_DRIVE_STATE_TIME;
    _timer.setTimeout(ms);
    _timer.start();
    driveDifferential(velocity, turn);
}

void DriveController::driveDistance(int16_t velocity, double meters) {
    if (meters <= 0)
        return;
    _autoState = AUTO_DRIVE_STATE_DISTANCE;
    _encoderRight.reset();
    _encoderLeft.reset();
    _targetTicks = metersToTicks(meters, _circumference, _slots);
    driveDifferential(velocity, 0);
}

/* TODO */
void DriveController::rotate(int16_t turn, double angle) {
    if (angle <= 0)
        return;

    // _autoState = AUTO_DRIVE_STATE_ROTATION;

    // _startHeading = _compass.getCompassDegree();

    driveDifferential(0, turn);
}

double DriveController::getDistanceTicks() const { return (_encoderRight.getCount() + _encoderLeft.getCount()) / 2.00; }

double DriveController::getDistanceMeters() const { return ticksToMeters(getDistanceTicks(), _circumference, _slots); }

uint32_t DriveController::getDurationMs() const { return _stopwatch.lap(); }

float DriveController::getHeading() const { return _compass.getCompassDegree(); }

uint8_t DriveController::getRightPWM() const { return _motorRight.getPWM(); }

uint8_t DriveController::getLeftPWM() const { return _motorLeft.getPWM(); }

const char* DriveController::getAutoState() const {
    switch (_autoState) {
        case AUTO_DRIVE_STATE_IDLE:     return "idle";
        case AUTO_DRIVE_STATE_TIME:     return "time";
        case AUTO_DRIVE_STATE_DISTANCE: return "distance";
        case AUTO_DRIVE_STATE_ROTATION: return "rotation";
    }
}

const char* DriveController::getMode() const {
    switch (_mode) {
        case DRIVE_MODE_AUTO:   return "auto";
        case DRIVE_MODE_MANUAL: return "manual";
    }
}

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

void DriveController::setModeManual() { 
    _mode = DRIVE_MODE_MANUAL; 
    _autoState = AUTO_DRIVE_STATE_IDLE;
}

bool DriveController::isModeAuto() const { return _mode == DRIVE_MODE_AUTO; }

bool DriveController::isModeManual() const { return _mode == DRIVE_MODE_MANUAL; }

bool DriveController::isDriving() const { return _motorRight.getPWM() > 0 || _motorLeft.getPWM() > 0; }
