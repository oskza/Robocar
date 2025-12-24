#include "MotionController.h"

MotionController::MotionController(Motor *motorR, Motor *motorL, 
                                    Encoder *encoderR, Encoder *encoderL, Timer *timer) 
                                : _motorR(motorR), _motorL(motorL), _encoderR(encoderR), _encoderL(encoderL), 
                                    _timer(timer), _mode(MOTION_MODE_AUTO), _isMoving(false), _ticksToMove(0) {}

void MotionController::_move(int16_t velocity, int16_t turn) {
    int16_t pwmR = constrain(velocity + turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    int16_t pwmL = constrain(velocity - turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    _motorR->rotate(pwmR); 
    _motorL->rotate(pwmL);
    _isMoving = true;
}

bool MotionController::validateMode(uint8_t mode) {
    return mode == MOTION_MODE_AUTO || mode == MOTION_MODE_MANUAL;
}

void MotionController::init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res) {
    _motorR->init(freq, res);
    _motorL->init(freq, res);
    _encoderR->init(onRightEncoder);
    _encoderL->init(onLeftEncoder);
    _timer->reset();
    _mode = MOTION_MODE_AUTO;
    _isMoving = false;
    _ticksToMove = 0;
}

void MotionController::tick() {
    if (!_isMoving || _mode == MOTION_MODE_MANUAL)
        return;

    if (_timer->isRunning() && _timer->tick()) {
        stop();
        _timer->reset();
        return;
    }
    
    if (_ticksToMove > 0 
            && (_encoderR->getCount() >= _ticksToMove || _encoderL->getCount() >= _ticksToMove)) {
        stop();
        _ticksToMove = 0;
        return;
    }
}

void MotionController::stop() {
    _motorR->stop(); 
    _motorL->stop();
    _encoderR->reset();
    _encoderL->reset();
    _isMoving = false;
}

void MotionController::moveManual(int16_t velocity, int16_t turn) {
    if (_mode == MOTION_MODE_AUTO)
        return;
    _move(velocity, turn);
}

void MotionController::moveForMs(int16_t velocity, int16_t turn, uint32_t ms) {
    if (_mode != MOTION_MODE_AUTO || _isMoving)
        return;
    _timer->setTimeout(ms);
    _timer->start();
    _move(velocity, turn);
}

void MotionController::moveForMeters(int16_t velocity, int16_t turn, float meters) {
    if (_mode != MOTION_MODE_AUTO || _isMoving)
        return;
    _ticksToMove = METERS_TO_TICKS(meters);
    _move(velocity, turn);
}

void MotionController::moveForward(uint8_t velocity) {
    if (_mode != MOTION_MODE_MANUAL)
        return;
    _move(velocity, 0);

}

void MotionController::moveBackward(uint8_t velocity) {
    if (_mode != MOTION_MODE_MANUAL)
        return;
    _move(-velocity, 0);

}

void MotionController::moveLeft(uint8_t velocity) {
    if (_mode != MOTION_MODE_MANUAL)
        return;
    _move(0, velocity);

}

void MotionController::moveRight(uint8_t velocity) {
    if (_mode != MOTION_MODE_MANUAL)
        return;
    _move(0, -velocity);
}

bool MotionController::isMoving() const { return _isMoving; }

bool MotionController::isModeAuto() const { return _mode == MOTION_MODE_AUTO; }

bool MotionController::isModeManual() const { return _mode == MOTION_MODE_MANUAL; }

void MotionController::setMode(uint8_t mode) {
    if (validateMode(mode))
        _mode = mode;
}
