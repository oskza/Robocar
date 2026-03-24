#ifndef RGB_INDICATOR_H
#define RGB_INDICATOR_H
#include <Arduino.h>

#define RGB_INDIC_DEFAULT_INTENSITY 180

class RGBIndicator {
private:
    uint8_t _pinR;
    uint8_t _pinG;
    uint8_t _pinB;
    uint8_t _chanR;
    uint8_t _chanG;
    uint8_t _chanB;
    uint8_t _intensity;
public:
    RGBIndicator(uint8_t pinR, uint8_t pinG, uint8_t pinB, 
                    uint8_t chanR, uint8_t chanG, uint8_t chanB,
                    uint8_t intensity = RGB_INDIC_DEFAULT_INTENSITY);
    void init(uint32_t freq, uint8_t res);
    void init(uint32_t freq, uint8_t res, uint8_t intensity);
    void off();
    void setIntensity(uint8_t intensity);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void error();
    void success();
    void waiting();
};
#endif
