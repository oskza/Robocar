#include "PowerController.h"

PowerController::PowerController(Ina226PowerMonitor &monitor)
    : _monitor(monitor) {}

bool PowerController::begin(float maxCurrentAmps, float shuntOhms) {
    return _monitor.begin(maxCurrentAmps, shuntOhms);
}

bool PowerController::isConnected() const {
    return _monitor.isConnected();
}

PowerSnapshot PowerController::getSnapshot() const {
    PowerSnapshot snapshot{};
    snapshot.connected = _monitor.isConnected();
    snapshot.busVoltage = _monitor.getBusVoltage();
    snapshot.currentMilliamps = _monitor.getCurrentMa();
    snapshot.powerMilliwatts = _monitor.getPowerMw();
    return snapshot;
}
