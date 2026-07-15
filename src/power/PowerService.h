#ifndef POWER_SERVICE_H
#define POWER_SERVICE_H
#include <Ina226PowerMonitor.h>
#include "PowerSnapshot.h"

class PowerService {
private:
    static constexpr float POWER_PRESENT_THRESHOLD_VOLTS = 0.5f;
    Ina226PowerMonitor &_monitor;
public:
    explicit PowerService(Ina226PowerMonitor &monitor);
    bool begin(float maxCurrentAmps, float shuntOhms);
    PowerSnapshot getSnapshot() const;
};
#endif
