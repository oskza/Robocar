#include "Bmm150Compass.h"

Bmm150Compass::Bmm150Compass(TwoWire &wire, uint8_t address)
    : _sensor(&wire, address), _available(false) {}

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

bool Bmm150Compass::readHeadingDegrees(float &headingDegrees) {
    if (!_available)
        return false;
    headingDegrees = _sensor.getCompassDegree();
    return true;
}
