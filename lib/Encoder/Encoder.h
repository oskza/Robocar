#ifndef ENCODER_H
#define ENCODER_H
#include <Arduino.h>

class Encoder {
private:
    uint8_t _pin;
    volatile uint32_t _count;
public:
    Encoder(uint8_t pin);
    void begin(void (*isr)());
    void IRAM_ATTR tick();
    uint32_t getCount() const;
    void reset();
};
#endif
