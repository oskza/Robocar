#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H
#include <Arduino.h>

class LEDIndicator {
private:
    uint8_t _pinRed;
    uint8_t _pinGreen;
    uint8_t _pinBlue;
    uint8_t _chanRed;
    uint8_t _chanGreen;
    uint8_t _chanBlue;
    uint8_t _intensity;
public:
    LEDIndicator(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue, uint8_t chanRed, uint8_t chanGreen, uint8_t chanBlue);
    void init(uint32_t freq, uint8_t res);
    void off();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setIntensity(uint8_t intensity);
    void error();
    void success();
    void waiting();
};
#endif
