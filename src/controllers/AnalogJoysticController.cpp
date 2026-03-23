#include "AnalogJoysticController.h"

AnalogJoysticController::AnalogJoysticController(AnalogJoystic &joystic, AnalogJoysticStorage &storage, Timer &timer) 
                                                    : _joystic(joystic), _storage(storage), _timer(timer), _enabled(false) {}

int16_t AnalogJoysticController::mapToSignedPWM(uint16_t raw, uint16_t center, uint16_t deadzone) {
    if (raw < center - deadzone)
        return map(raw, center - deadzone, ANALOG_JOYSTIC_MIN_VALUE, 0, 255);
    if (raw > center + deadzone)
        return map(raw, center + deadzone, ANALOG_JOYSTIC_MAX_VALUE, 0, -255);
    return 0;
}

void AnalogJoysticController::init() {
    _storage.begin();
    AnalogJoysticConfig cfg;
    _storage.loadConfig(cfg);
    _joystic.setDeadzone(cfg.deadzone);
    _timer.setTimeout(cfg.intervalMs);
    _timer.reset();
    _enabled = false;
    _joystic.calibrateVerticalCenter();
    _joystic.calibrateHorizontalCenter();
}

bool AnalogJoysticController::tick(int16_t &vert, int16_t &horz) {
    if(!_timer.tick())
        return false;
    vert = readVerticalSignedPWM();
    horz = readHorizontalSignedPWM();
    _timer.refresh();
    return true;
}

int16_t AnalogJoysticController::readVerticalSignedPWM() const { 
    return (_enabled) 
            ? mapToSignedPWM(_joystic.readVerticalRaw(), 
                                _joystic.getVerticalCenter(), 
                                _joystic.getDeadzone()) 
            : 0;
}

int16_t AnalogJoysticController::readHorizontalSignedPWM() const {
    return (_enabled) 
            ? mapToSignedPWM(_joystic.readHorizontalRaw(), 
                                _joystic.getHorizontalCenter(), 
                                _joystic.getDeadzone()) 
            : 0;
}

void AnalogJoysticController::enable() {
    if (_enabled)
        return; 
    _enabled = true;
    _timer.start();
}

void AnalogJoysticController::disable() {
    if (!_enabled)
        return;
    _enabled = false;
    _timer.reset();
}

void AnalogJoysticController::getConfig(AnalogJoysticConfig &target) const { 
    target.deadzone = _joystic.getDeadzone(); 
    target.intervalMs = _timer.getTimeout(); 
}

void AnalogJoysticController::updateConfig(AnalogJoysticConfig &cfg) {
    updateDeadzone(cfg.deadzone);
    updateIntervalMs(cfg.intervalMs);
}

void AnalogJoysticController::resetConfig() { 
    _joystic.setDeadzone(AnalogJoysticDefaults::deadzone);
    _timer.setTimeout(AnalogJoysticDefaults::intervalMs);
    _storage.reset(); 
}

void AnalogJoysticController::updateDeadzone(uint16_t deadzone) {
    if (deadzone == _joystic.getDeadzone())
        return;
    _joystic.setDeadzone(deadzone);
    _storage.saveDeadzone(deadzone);
}

void AnalogJoysticController::updateIntervalMs(uint32_t intervalMs) {
    if (intervalMs == _timer.getTimeout())
        return;
    _timer.setTimeout(intervalMs);
    _storage.saveIntervalMs(intervalMs);
}
