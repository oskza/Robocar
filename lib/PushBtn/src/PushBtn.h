#ifndef PUSH_BTN_H
#define PUSH_BTN_H
#include <Arduino.h>
#include <Timer.h>

#define PUSH_BTN_DEFAULT_DEBOUNCE_MS 50

class PushBtn {
private:
    Timer &_timer;
    uint8_t _pin;
    bool _lastReading;
    void (*_onClick)();
public:
    PushBtn(Timer &timer, uint8_t pin);
    void init(uint32_t msDebounce = PUSH_BTN_DEFAULT_DEBOUNCE_MS, void (*onClick)() = nullptr);
    void tick();
    void onClick(void (*cb)());
};
#endif
