#include "Ina226PowerMonitor.h"

Ina226PowerMonitor::Ina226PowerMonitor(uint8_t address) : _ina(address), _connected(false) {}

bool Ina226PowerMonitor::begin(float maxCurrentAmps, float shuntOhms) {
    _connected = Wire.begin()
        && _ina.begin()
        && _ina.setMaxCurrentShunt(maxCurrentAmps, shuntOhms) == INA226_ERR_NONE
        && _ina.setAverage(INA226_16_SAMPLES);
    return _connected;
}

bool Ina226PowerMonitor::isConnected() const { return _connected; }

float Ina226PowerMonitor::getBusVoltage() { return (_connected) ? _ina.getBusVoltage() : 0.0f; }

float Ina226PowerMonitor::getShuntVoltageMv() { return (_connected) ? _ina.getShuntVoltage_mV() : 0.0f; }

float Ina226PowerMonitor::getCurrentMa() { return (_connected) ? _ina.getCurrent_mA() : 0.0f; }

float Ina226PowerMonitor::getPowerMw() { return (_connected) ? _ina.getPower_mW() : 0.0f; }
