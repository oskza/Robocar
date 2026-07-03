#include "Bmm150Compass.h"
#include <Wire.h>

Bmm150Compass::Bmm150Compass() : _sensor(&Wire, I2C_ADDRESS_4), _available(false) {}

bool Bmm150Compass::begin() {
    if (_sensor.begin() != 0) {
        _available = false;
        return false;
    }
    _sensor.setOperationMode(BMM150_POWERMODE_NORMAL);
    _sensor.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    _sensor.setRate(BMM150_DATA_RATE_10HZ);
    _sensor.setMeasurementXYZ();
    _available = true;
    return true;
}

bool Bmm150Compass::isAvailable() const { return _available; }

float Bmm150Compass::getHeadingDegrees() { return _sensor.getCompassDegree(); }
