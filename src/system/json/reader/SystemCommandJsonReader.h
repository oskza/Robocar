#ifndef SYSTEM_COMMAND_JSON_READER_H
#define SYSTEM_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "../../SystemCommand.h"

namespace SystemCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, SystemCommand &out);
}
#endif
