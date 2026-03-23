#ifndef COMPASS_CONTROLLER
#define COMPASS_CONTROLLER
#include <DFRobot_BMM150.h>

class CompassController {
private:
    DFRobot_BMM150_I2C &_compass;
public:
    CompassController(DFRobot_BMM150_I2C &compass);
    bool init();
    float getHeading() const;
};
#endif
