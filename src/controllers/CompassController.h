#ifndef COMPASS_CONTROLLER_H
#define COMPASS_CONTROLLER_H
#include <DFRobot_BMM150.h>

// values @ DFRobot_BMM150
#define COMPASS_ERROR_CODE_NONE         0
#define COMPASS_ERROR_CODE_CONNECTION   1
#define COMPASS_ERROR_CODE_CHIP_ID      2

class CompassController {
private:
    DFRobot_BMM150_I2C &_compass;
    bool _isReady;
    uint8_t _error;
public:
    CompassController(DFRobot_BMM150_I2C &compass);
    bool init();
    float getHeading() const;
    bool isReady() const;
    bool hasError() const;
    uint8_t getError() const;
};
#endif
