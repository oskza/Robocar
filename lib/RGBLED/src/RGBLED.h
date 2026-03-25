#ifndef RGB_LED_H
#define RGB_LED_H
#include <Arduino.h>

class RGBLED {
private:
    uint8_t _pinR, _pinG, _pinB, _chanR, _chanG, _chanB;
    void _initPin(uint8_t pin, uint8_t chan, uint32_t freq, uint8_t res);
public:
    RGBLED(uint8_t pinR, uint8_t pinG, uint8_t pinB, 
                    uint8_t chanR, uint8_t chanG, uint8_t chanB);
    void init(uint32_t freq, uint8_t res);
    void off();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
};
#endif
