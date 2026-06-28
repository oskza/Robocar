#ifndef BMM150_COMPASS_H
#define BMM150_COMPASS_H
#include <Arduino.h>
#include <DFRobot_BMM150.h>

class Bmm150Compass {
private:
    DFRobot_BMM150_I2C _sensor;
public:
    Bmm150Compass();
    bool begin();
    float getHeadingDegrees();
};
#endif
