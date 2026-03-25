#include "PowerController.h"

PowerController::PowerController(INA226 &ina) : _ina(ina), _isReady(false), _error(POWER_ERROR_CODE_NONE) {}

bool PowerController::init(float maxCurrent, float shuntOhm) {
    if (isReady() && !hasError())
        return true;
    if (!Wire.begin() || !_ina.begin()) {
        _error = POWER_ERROR_CODE_INIT_FAIL;
        _isReady = false;
        return false;
    }
    _ina.setAverage(INA226_16_SAMPLES);
    if (_ina.setMaxCurrentShunt(maxCurrent, shuntOhm) != 0) {
        _error = POWER_ERROR_CODE_CALIBR_FAIL;
        _isReady = false;
        return false;
    }
    _error = POWER_ERROR_CODE_NONE;
    _isReady = true;
    return true;
}

float PowerController::getVoltage() { return isReady() ? _ina.getBusVoltage() : 0; }

bool PowerController::isReady() const { return _isReady; }

bool PowerController::hasError() const { return _error != POWER_ERROR_CODE_NONE; }

uint8_t PowerController::getError() const { return _error; }
