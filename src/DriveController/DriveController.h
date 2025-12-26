#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
#include <Motor.h>

class DriveController {
private:
    Motor &_right;
    Motor &_left;
    bool _isDriving;
public:
    DriveController(Motor &right, Motor &left);
    void init(uint32_t freq, uint8_t res);
    void stop();
    void driveDifferential(int16_t velocity, int16_t turn);
    void driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left);
    bool isDriving() const;
};
#endif
