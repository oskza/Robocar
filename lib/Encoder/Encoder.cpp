#include "Encoder.h"

Encoder::Encoder(uint8_t pin) : _pin(pin), _count(0) {}

void Encoder::begin(void (*isr)()) {
    pinMode(_pin, INPUT_PULLUP);
    _count = 0;
    attachInterrupt(digitalPinToInterrupt(_pin), isr, FALLING);
}

void IRAM_ATTR Encoder::tick() { _count++; }

uint32_t Encoder::getCount() const {
    noInterrupts();
    uint32_t c = _count;
    interrupts();
    return c;
}

void Encoder::reset() {
    noInterrupts();
    _count = 0;
    interrupts();
}
