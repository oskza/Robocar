#include "CompassController.h"

CompassController::CompassController(DFRobot_BMM150_I2C &compass) 
                                        : _compass(compass), _isReady(false), 
                                            _error(COMPASS_ERROR_CODE_NONE) {}

bool CompassController::init() {
    if (isReady() && !hasError())
        return true;
    _error = _compass.begin();
    if(_error) {
        _isReady = false;
        return false;
    }
    _compass.setOperationMode(BMM150_POWERMODE_NORMAL);
    _compass.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    _compass.setRate(BMM150_DATA_RATE_10HZ);
    _compass.setMeasurementXYZ();
    _isReady = true;
    return true;
}

float CompassController::getHeading() const { 
    return _isReady ? _compass.getCompassDegree() : 0;
}

bool CompassController::isReady() const { return _isReady; }

bool CompassController::hasError() const { return _error != COMPASS_ERROR_CODE_NONE; }

uint8_t CompassController::getError() const { return _error; }
