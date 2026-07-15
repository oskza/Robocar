#include "PowerService.h"

PowerService::PowerService(Ina226PowerMonitor &monitor) : _monitor(monitor) {}

bool PowerService::begin(float maxCurrentAmps, float shuntOhms) {
    return _monitor.begin(maxCurrentAmps, shuntOhms);
}

PowerSnapshot PowerService::getSnapshot() const {
    PowerSnapshot snapshot{};
    snapshot.connected = _monitor.isConnected();
    if (!snapshot.connected)
        return snapshot;
    snapshot.volts = _monitor.getBusVoltage();
    snapshot.hasPower = snapshot.volts > POWER_PRESENT_THRESHOLD_VOLTS;
    if (!snapshot.hasPower)
        return snapshot;
    snapshot.milliamps = _monitor.getCurrentMa();
    snapshot.milliwatts = _monitor.getPowerMw();
    return snapshot;
}
