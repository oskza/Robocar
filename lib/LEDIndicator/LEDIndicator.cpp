#include "LEDIndicator.h"

LEDIndicator::LEDIndicator(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue, 
                                uint8_t chanRed, uint8_t chanGreen, uint8_t chanBlue) 
                            : _pinRed(pinRed), _pinGreen(pinGreen), _pinBlue(pinBlue), 
                                _chanRed(chanRed), _chanGreen(chanGreen), _chanBlue(chanBlue),
                                _intensity(180) {}

void LEDIndicator::init(uint32_t freq, uint8_t res) {
    ledcSetup(_chanRed, freq, res);
    ledcSetup(_chanGreen, freq, res);
    ledcSetup(_chanBlue, freq, res);
    ledcAttachPin(_pinRed, _chanRed);
    ledcAttachPin(_pinGreen, _chanGreen);
    ledcAttachPin(_pinBlue, _chanBlue);
    off();
}

void LEDIndicator::off() {
    ledcWrite(_chanRed, 0);
    ledcWrite(_chanGreen, 0);
    ledcWrite(_chanBlue, 0);
}

void LEDIndicator::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    ledcWrite(_chanRed, red);
    ledcWrite(_chanGreen, green);
    ledcWrite(_chanBlue, blue);
}

void LEDIndicator::setIntensity(uint8_t intensity) { _intensity = intensity; }

void LEDIndicator::error() { setColor(_intensity, 0, 0); }

void LEDIndicator::success() { setColor(0, _intensity, 0); }

void LEDIndicator::waiting() { setColor(0, 0, _intensity); }
