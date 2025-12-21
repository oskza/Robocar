#include "MotionController.h"

MotionController::MotionController(Motor *motorR, Motor *motorL, 
                                    Encoder *encoderR, Encoder *encoderL, Timer *timer) 
                                : _motorR(motorR), _motorL(motorL), 
                                    _encoderR(encoderR), _encoderL(encoderL), _timer(timer) {}

void MotionController::init(void (*onRightEncoder)(), void (*onLeftEncoder)(), uint32_t freq, uint8_t res) {
    _motorR->init(freq, res);
    _motorL->init(freq, res);
    _encoderR->init(onRightEncoder);
    _encoderL->init(onLeftEncoder);
}

void MotionController::stop() {
    _motorR->stop(); 
    _motorL->stop();
    _encoderR->reset();
    _encoderL->reset();
    _timer->reset();
}

void MotionController::move(int16_t velocity, int16_t turn) {
    int16_t pwmR = constrain(velocity + turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    int16_t pwmL = constrain(velocity - turn, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    _motorR->rotate(pwmR); 
    _motorL->rotate(pwmL);
}

bool MotionController::moveForMs(int16_t velocity, int16_t turn, uint32_t ms) {
    if(_timer->tick()) {
        stop();
        return true;
    }
    if(!_timer->isRunning()) {
        _timer->setTimeout(ms);
        _timer->start();
    }
    move(velocity, turn);
    return false;
}

bool MotionController::moveForMeters(int16_t velocity, int16_t turn, float meters) {
    uint32_t ticks = METERS_TO_TICKS(meters);
    if(_encoderR->getCount() >= ticks 
            || _encoderL->getCount() >= ticks) {
        stop();
        return true;
    }
    move(velocity, turn);
    return false;
}
