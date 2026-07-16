#ifndef MOTION_CONFIG_JSON_WRITER_H
#define MOTION_CONFIG_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../MotionConfig.h"

namespace MotionConfigJsonWriter {
    void write(JsonObject json, const MotionConfig &config);
}
#endif
