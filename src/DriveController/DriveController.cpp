#include "DriveController.h"

DriveController::DriveController(Motor &right, Motor &left, Timer &timer) 
                                : _right(right), _left(left), _timer(timer), _isDriving(false) {}

void DriveController::init(uint32_t freq, uint8_t res) {
    _right.init(freq, res);
    _left.init(freq, res);
    _timer.reset();
    _isDriving = false;
}

void DriveController::tick() {
    if (!_isDriving || !_timer.isRunning() || !_timer.tick())
        return;
    stop();
    _timer.reset();
}

void DriveController::stop() {
    if (!_isDriving)
        return;
    _right.stop(); 
    _left.stop();
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
    _right.setSignedPWM(pwmRight);
    _left.setSignedPWM(pwmLeft);
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
    // ...
}

bool DriveController::isDriving() const { return _isDriving; }
