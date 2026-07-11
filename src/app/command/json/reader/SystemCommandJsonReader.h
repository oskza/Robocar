#ifndef SYSTEM_COMMAND_JSON_READER_H
#define SYSTEM_COMMAND_JSON_READER_H

#include <ArduinoJson.h>
#include "../../common/CommandEnvelope.h"

class SystemCommandJsonReader {
public:
    static bool read(
        JsonObjectConst json,
        CommandEnvelope &command
    );
};

#endif
