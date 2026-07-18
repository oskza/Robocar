#ifndef ROBOT_COMMAND_JSON_READER_H
#define ROBOT_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../RobotCommand.h"

namespace RobotCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, RobotCommand &out);
}
#endif
