#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>
#include <Timer.h>

class DriveController {
private:
    Motor &_right;
    Motor &_left;
    Timer &_timer;
    bool _isDriving;
public:
    DriveController(Motor &right, Motor &left, Timer &timer);
    void init(uint32_t freq, uint8_t res);
    void tick();
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left);
    void driveFor(int16_t velocity, int16_t turn, uint32_t ms);
    void driveDistance(int16_t velocity, float meters);
    bool isDriving() const;
};
#endif
