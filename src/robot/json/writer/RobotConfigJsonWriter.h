#ifndef ROBOT_CONFIG_JSON_WRITER_H
#define ROBOT_CONFIG_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../RobotConfig.h"

namespace RobotConfigJsonWriter {
    void write(JsonObject json, const RobotConfig &config);
}
#endif
