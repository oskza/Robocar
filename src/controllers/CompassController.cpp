#include "CompassController.h"

CompassController::CompassController(DFRobot_BMM150_I2C &compass) : _compass(compass) {}

bool CompassController::init() {
    if(_compass.begin())
        return false;
    _compass.setOperationMode(BMM150_POWERMODE_NORMAL);
    _compass.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    _compass.setRate(BMM150_DATA_RATE_10HZ);
    _compass.setMeasurementXYZ();
    return true;
}

float CompassController::getHeading() const { return _compass.getCompassDegree(); }
