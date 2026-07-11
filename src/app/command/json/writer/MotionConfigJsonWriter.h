#ifndef MOTION_CONFIG_JSON_WRITER_H
#define MOTION_CONFIG_JSON_WRITER_H

#include <ArduinoJson.h>
#include "../../../../subsystems/motion/MotionConfig.h"

class MotionConfigJsonWriter {
public:
    static void write(JsonObject json, const MotionConfig &config);
};

#endif
