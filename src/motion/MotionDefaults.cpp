#include "MotionDefaults.h"

namespace MotionDefaults {
    MotionConfig config() {
        MotionConfig cfg{};
        cfg.wheelDiameterMeters = 0.067f;
        cfg.wheelCircumferenceFactor = 1.0f;
        cfg.wheelAcceleration = 5;
        cfg.motorLeftMinPwm = 110;
        cfg.motorRightMinPwm = 110;
        cfg.headingToleranceDegrees = 12.0f;
        return cfg;
    }
}
