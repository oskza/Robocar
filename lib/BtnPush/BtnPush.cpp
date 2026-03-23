#include "BtnPush.h"

BtnPush::BtnPush(Timer &timer, uint8_t pin) 
                    : _timer(timer), _pin(pin), _lastReading(HIGH), _onClickCb(nullptr) {}

void BtnPush::init(uint32_t debounceDelay, void (*onClickCb)()) {
    pinMode(_pin, INPUT_PULLUP);
    _onClickCb = onClickCb;
    _timer.setTimeout(debounceDelay);
    _timer.reset();
}

void BtnPush::onClick(void (*cb)()) { _onClickCb = cb; }

void BtnPush::tick() {
    bool reading = digitalRead(_pin);
    if (reading != _lastReading) {
        _timer.start();
        _lastReading = reading;
        return;
    }
    if (_timer.tick()) {
        _timer.reset(); 
        if (_lastReading == HIGH && _onClickCb)
            _onClickCb();
    }
}
