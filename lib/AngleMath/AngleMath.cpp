#include "AngleMath.h"
#include <math.h>

float AngleMath::normalizeDegrees(float degrees) {
    degrees = fmodf(degrees, 360.0f);
    return (degrees < 0.0f) ? degrees + 360.0f : degrees;
}

float AngleMath::differenceDegrees(float fromDegrees, float toDegrees) {
    float diff = normalizeDegrees(toDegrees) - normalizeDegrees(fromDegrees);
    if (diff > 180.0f)
        diff -= 360.0f;
    else if (diff < -180.0f)
        diff += 360.0f;
    return diff;
}
