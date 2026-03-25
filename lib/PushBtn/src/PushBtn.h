#ifndef PUSH_BTN_H
#define PUSH_BTN_H
#include <Arduino.h>

#define PUSH_BTN_DEFAULT_DEBOUNCE_TIMEOUT   50
#define PUSH_BTN_DEFAULT_LONG_PRESS_TIMEOUT 3000

class PushBtn {
private:
    uint8_t _pin;
    uint32_t _debounceTimeout;
    uint32_t _longPressTimeout;
    uint32_t _lastDebounceMs;
    uint32_t _pressStartMs;
    bool _lastReading;
    bool _stableState;
    bool _isPressed;
    bool _longPressTriggered;
    void (*_onClick)();
    void (*_onLongPress)();
public:
    PushBtn(uint8_t pin);
    void init();
    void tick();
    void onClick(void (*cb)());
    void onLongPress(void (*cb)());
    void setDebounceTimeout(uint32_t timeout);
    void setLongPressTimeout(uint32_t timeout);
};
#endif
