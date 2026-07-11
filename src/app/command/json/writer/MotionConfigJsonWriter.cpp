#include "MotionConfigJsonWriter.h"

void MotionConfigJsonWriter::write(JsonObject json, const MotionConfig &config) {
    json["wheelDiameterMeters"] = config.wheelDiameterMeters;
    json["wheelCircumferenceFactor"] = config.wheelCircumferenceFactor;
    json["wheelAcceleration"] = config.wheelAcceleration;

    json["motorLeftMinPwm"] = config.motorLeftMinPwm;
    json["motorRightMinPwm"] = config.motorRightMinPwm;

    json["headingToleranceDegrees"] = config.headingToleranceDegrees;
}
