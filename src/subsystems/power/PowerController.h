#ifndef POWER_CONTROLLER_H
#define POWER_CONTROLLER_H
#include "PowerSnapshot.h"
#include <Ina226PowerMonitor.h>

class PowerController {
private:
    Ina226PowerMonitor &_monitor;
public:
    explicit PowerController(Ina226PowerMonitor &monitor);
    bool begin(float maxCurrentAmps, float shuntOhms);
    bool isConnected() const;
    PowerSnapshot getSnapshot() const;
};
#endif
