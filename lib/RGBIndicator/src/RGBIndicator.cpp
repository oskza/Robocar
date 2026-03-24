#include "RGBIndicator.h"

RGBIndicator::RGBIndicator(uint8_t pinR, uint8_t pinG, uint8_t pinB, 
                                uint8_t chanR, uint8_t chanG, uint8_t chanB,
                                uint8_t intensity) 
                            : _pinR(pinR), _pinG(pinG), _pinB(pinB), 
                                _chanR(chanR), _chanG(chanG), _chanB(chanB),
                                _intensity(intensity) {}

void RGBIndicator::init(uint32_t freq, uint8_t res) {
    ledcSetup(_chanR, freq, res);
    ledcAttachPin(_pinR, _chanR);
    ledcSetup(_chanG, freq, res);
    ledcAttachPin(_pinG, _chanG);
    ledcSetup(_chanB, freq, res);
    ledcAttachPin(_pinB, _chanB);
    off();
}

void RGBIndicator::init(uint32_t freq, uint8_t res, uint8_t intensity) {
    init(freq, res);
    setIntensity(intensity);
}

void RGBIndicator::off() {
    ledcWrite(_chanR, 0);
    ledcWrite(_chanG, 0);
    ledcWrite(_chanB, 0);
}

void RGBIndicator::setIntensity(uint8_t intensity) { _intensity = intensity; }

void RGBIndicator::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    ledcWrite(_chanR, red);
    ledcWrite(_chanG, green);
    ledcWrite(_chanB, blue);
}

void RGBIndicator::error() { setColor(_intensity, 0, 0); }

void RGBIndicator::success() { setColor(0, _intensity, 0); }

void RGBIndicator::waiting() { setColor(0, 0, _intensity); }
