#include "DriveController.h"

DriveController::DriveController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, Timer &timer) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _timer(timer), _isDriving(false), _targetTicks(0) {}

void DriveController::init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res) {
    _motorRight.init(freq, res);
    _motorLeft.init(freq, res);
    _encoderRight.init(onRightEncoder);
    _encoderLeft.init(onLeftEncoder);
    _timer.reset();
    _isDriving = false;
    _targetTicks = 0;
}

void DriveController::tick() {
    if (!_isDriving)
        return;
    if (_timer.isRunning() && _timer.tick()) {
        stop();
        _timer.reset();
        return;
    }
    if (_targetTicks > 0 
            && _targetTicks <= ((_encoderRight.getCount() + _encoderLeft.getCount()) / 2)) {
        stop();
        _targetTicks = 0;
    }
}

void DriveController::stop() {
    if (!_isDriving)
        return;
    _motorRight.stop(); 
    _motorLeft.stop();
    _encoderRight.reset();
    _encoderLeft.reset();
    _isDriving = false;
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
    _isDriving = true;
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

bool DriveController::isDriving() const { return _isDriving; }
