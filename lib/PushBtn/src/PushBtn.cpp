#include "PushBtn.h"

PushBtn::PushBtn(uint8_t pin)
                    : _pin(pin), _debounceTimeout(PUSH_BTN_DEFAULT_DEBOUNCE_TIMEOUT), 
                        _longPressTimeout(PUSH_BTN_DEFAULT_LONG_PRESS_TIMEOUT),
                        _lastDebounceMs(0), _pressStartMs(0),
                        _lastReading(HIGH), _stableState(HIGH),
                        _isPressed(false), _longPressTriggered(false),
                        _onClick(nullptr), _onLongPress(nullptr) {}

void PushBtn::init() { pinMode(_pin, INPUT_PULLUP); }

void PushBtn::tick() {
    bool reading = digitalRead(_pin);
    if (reading != _lastReading) {
        _lastDebounceMs = millis();
        _lastReading = reading;
    }
    if ((millis() - _lastDebounceMs) > _debounceTimeout) {
        if (reading != _stableState) {
            _stableState = reading;
            if (_stableState == LOW) {
                _isPressed = true;
                _longPressTriggered = false;
                _pressStartMs = millis();
            } else {
                _isPressed = false;
                if (!_longPressTriggered && _onClick)
                    _onClick();
            }
        }
    }
    if (_isPressed && !_longPressTriggered) {
        if (millis() - _pressStartMs >= _longPressTimeout) {
            _longPressTriggered = true;
            if (_onLongPress)
                _onLongPress();
        }
    }
}

void PushBtn::onClick(void (*cb)()) { _onClick = cb; }

void PushBtn::onLongPress(void (*cb)()) { _onLongPress = cb; }

void PushBtn::setDebounceTimeout(uint32_t timeout) { _debounceTimeout = timeout; }
void PushBtn::setLongPressTimeout(uint32_t timeout) { _longPressTimeout = timeout; }