#include "Timer.h"

Timer::Timer() : _timeMs(0), _running(false) {}

void Timer::start(uint32_t nowMs) {
    _timeMs = nowMs;
    _running = true;
}

void Timer::restart(uint32_t nowMs) { _timeMs = nowMs; }

void Timer::stop() {
    _running = false;
    _timeMs = 0;
}

bool Timer::isRunning() const { return _running; }

uint32_t Timer::getElapsedTime(uint32_t nowMs) const { return nowMs - _timeMs; }

bool Timer::hasElapsed(uint32_t nowMs, uint32_t durationMs) const {
    return _running && getElapsedTime(nowMs) >= durationMs;
}

bool Timer::poll(uint32_t nowMs, uint32_t intervalMs) {
    if (!_running) {
        start(nowMs);
        return false;
    }
    if (!hasElapsed(nowMs, intervalMs))
        return false;
    restart(nowMs);
    return true;
}
