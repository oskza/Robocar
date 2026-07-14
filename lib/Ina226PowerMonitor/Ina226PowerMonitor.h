#ifndef INA226_POWER_MONITOR_H
#define INA226_POWER_MONITOR_H
#include <INA226.h>

class Ina226PowerMonitor {
private:
    INA226 _ina;
    bool _connected;
public:
    explicit Ina226PowerMonitor(uint8_t address = 0x40);
    bool begin(float maxCurrentAmps, float shuntOhms);
    bool isConnected() const;
    float getBusVoltage();
    float getShuntVoltageMv();
    float getCurrentMa();
    float getPowerMw();
};
#endif
