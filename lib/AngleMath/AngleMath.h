#ifndef ANGLE_MATH_H
#define ANGLE_MATH_H

#include <Arduino.h>

class AngleMath {
public:
    static float normalizeDegrees(float degrees);
    static float differenceDegrees(float fromDegrees, float toDegrees);
};

#endif
