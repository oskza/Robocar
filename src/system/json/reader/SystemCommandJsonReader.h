#ifndef SYSTEM_COMMAND_JSON_READER_H
#define SYSTEM_COMMAND_JSON_READER_H
#include <ArduinoJson.h>
#include "command/CommandEnvelope.h"

namespace SystemCommandJsonReader {
    bool read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command);
}
#endif
