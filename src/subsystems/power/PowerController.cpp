#include "PowerController.h"

PowerController::PowerController(Ina226PowerMonitor &monitor) : _monitor(monitor) {}

bool PowerController::begin(float maxCurrentAmps, float shuntOhms) {
    return _monitor.begin(maxCurrentAmps, shuntOhms);
}

bool PowerController::isConnected() const { return _monitor.isConnected(); }

bool PowerController::hasPower() const { return _monitor.getBusVoltage() > 0.5f; }

PowerSnapshot PowerController::getSnapshot() const {
    PowerSnapshot snapshot{};
    snapshot.connected = _monitor.isConnected();
    if (!snapshot.connected)
        return snapshot;
    snapshot.volts = _monitor.getBusVoltage();
    snapshot.hasPower = snapshot.volts > 0.5f;
    if (!snapshot.hasPower)
        return snapshot;
    snapshot.milliamps = _monitor.getCurrentMa();
    snapshot.milliwatts = _monitor.getPowerMw();
    return snapshot;
}
