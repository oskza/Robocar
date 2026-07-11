#ifndef ROBOT_CONFIG_JSON_WRITER_H
#define ROBOT_CONFIG_JSON_WRITER_H

#include <ArduinoJson.h>
#include "../../../../robot/RobotConfig.h"

class RobotConfigJsonWriter {
public:
    static void write(JsonObject json, const RobotConfig &config);
};

#endif
