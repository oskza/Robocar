#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

class Timer {
private:
    uint32_t _timeMs;
    bool _running;
public:
    Timer();
    void start(uint32_t nowMs);
    void stop();
    void restart(uint32_t nowMs);
    bool isRunning() const;
    uint32_t getElapsedTime(uint32_t nowMs) const;
    bool hasElapsed(uint32_t nowMs, uint32_t durationMs) const;
    bool poll(uint32_t nowMs, uint32_t intervalMs);
};
#endif
