#include "RGBLED.h"

RGBLED::RGBLED(uint8_t pinR, uint8_t pinG, uint8_t pinB, 
                                uint8_t chanR, uint8_t chanG, uint8_t chanB) 
                            : _pinR(pinR), _pinG(pinG), _pinB(pinB), 
                                _chanR(chanR), _chanG(chanG), _chanB(chanB) {}

void RGBLED::_initPin(uint8_t pin, uint8_t chan, uint32_t freq, uint8_t res) {
    ledcSetup(chan, freq, res);
    ledcAttachPin(pin, chan);
}

void RGBLED::init(uint32_t freq, uint8_t res) {
    _initPin(_pinR, _chanR, freq, res);
    _initPin(_pinG, _chanG, freq, res);
    _initPin(_pinB, _chanB, freq, res);
    off();
}

void RGBLED::off() { setColor(0, 0, 0); }

void RGBLED::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    ledcWrite(_chanR, red);
    ledcWrite(_chanG, green);
    ledcWrite(_chanB, blue);
}
