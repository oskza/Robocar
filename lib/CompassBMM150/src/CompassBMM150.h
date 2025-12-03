#ifndef COMPASS_BMM150_H
#define COMPASS_BMM150_H
#include <DFRobot_BMM150.h>

class CompassBMM150 : public DFRobot_BMM150_I2C {
public:
    CompassBMM150();
    bool init();
};
#endif
