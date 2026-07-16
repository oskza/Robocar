#ifndef MOTION_COMMAND_JSON_READER_H
#define MOTION_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../MotionCommand.h"

namespace MotionCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, MotionCommand &out);
}
#endif
