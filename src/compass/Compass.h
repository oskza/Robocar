#ifndef COMPASS_H
#define COMPASS_H
#include <DFRobot_BMM150.h>

class Compass {
private:
    DFRobot_BMM150_I2C &_bmm;
public:
    Compass(DFRobot_BMM150_I2C &bmm);
    bool init();
    float getHeading() const;
};
#endif
