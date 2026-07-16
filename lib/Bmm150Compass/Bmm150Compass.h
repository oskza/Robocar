#ifndef BMM150_COMPASS_H
#define BMM150_COMPASS_H
#include <DFRobot_BMM150.h>
#include <Wire.h>

class Bmm150Compass {
private:
    DFRobot_BMM150_I2C _sensor;
    bool _available;
public:
    explicit Bmm150Compass(TwoWire &wire = Wire, uint8_t address = I2C_ADDRESS_4);
    bool begin();
    bool isAvailable() const;
    bool readHeadingDegrees(float &headingDegrees);
};
#endif
