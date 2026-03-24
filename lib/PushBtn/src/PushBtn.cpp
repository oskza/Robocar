#include "PushBtn.h"

PushBtn::PushBtn(Timer &timer, uint8_t pin) 
                    : _timer(timer), _pin(pin), _lastReading(HIGH), _onClick(nullptr) {}

void PushBtn::init(uint32_t msDebounce, void (*onClick)()) {
    pinMode(_pin, INPUT_PULLUP);
    _onClick = onClick;
    _timer.setTimeout(msDebounce);
    _timer.reset();
}

void PushBtn::onClick(void (*cb)()) { _onClick = cb; }

void PushBtn::tick() {
    bool reading = digitalRead(_pin);
    if (reading != _lastReading) {
        _timer.start();
        _lastReading = reading;
        return;
    }
    if (_timer.tick()) {
        _timer.reset(); 
        if (_lastReading == HIGH && _onClick)
            _onClick();
    }
}
