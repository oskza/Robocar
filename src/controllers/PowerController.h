#ifndef POWER_CONTROLLER_H
#define POWER_CONTROLLER_H
#include <Arduino.h>
#include <INA226.h>

#define POWER_ERROR_CODE_NONE           0
#define POWER_ERROR_CODE_INIT_FAIL      1
#define POWER_ERROR_CODE_CALIBR_FAIL    2

class PowerController {
private:
    INA226 &_ina;
    bool _isReady;
    uint8_t _error;
public:
    PowerController(INA226 &ina);
    bool init(float maxCurrent, float shuntOhm);
    float getVoltage();
    bool isReady() const;
    bool hasError() const;
    uint8_t getError() const;
};
#endif
