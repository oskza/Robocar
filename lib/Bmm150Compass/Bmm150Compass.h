#ifndef BMM150_COMPASS_H
#define BMM150_COMPASS_H
#include <DFRobot_BMM150.h>

class Bmm150Compass {
private:
    DFRobot_BMM150_I2C _sensor;
    bool _available;
public:
    Bmm150Compass();
    bool begin();
    bool isAvailable() const;
    float getHeadingDegrees();
};
#endif
