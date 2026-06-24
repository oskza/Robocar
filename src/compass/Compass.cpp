#include "Compass.h"

Compass::Compass(DFRobot_BMM150_I2C &bmm) : _bmm(bmm) {}

bool Compass::init() {
    if (_bmm.begin())
        return false;
    _bmm.setOperationMode(BMM150_POWERMODE_NORMAL);
    _bmm.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    _bmm.setRate(BMM150_DATA_RATE_10HZ);
    _bmm.setMeasurementXYZ();
    return true;
}

float Compass::getHeading() const { return _bmm.getCompassDegree(); }
