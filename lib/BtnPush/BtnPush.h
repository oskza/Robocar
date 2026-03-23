#ifndef BTN_PUSH_H
#define BTN_PUSH_H
#include <Arduino.h>
#include <Timer.h>

class BtnPush {
private:
    Timer &_timer;
    uint8_t _pin;
    bool _lastReading;
    void (*_onClickCb)();
public:
    BtnPush(Timer &timer, uint8_t pin);
    void init(uint32_t debounceDelay, void (*onClickCb)() = nullptr);
    void tick();
    void onClick(void (*cb)());
};
#endif
