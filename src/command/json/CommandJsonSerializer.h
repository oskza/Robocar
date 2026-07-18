#ifndef COMMAND_JSON_SERIALIZER_H
#define COMMAND_JSON_SERIALIZER_H
#include <stddef.h>
#include "command/CommandResponse.h"

namespace CommandJsonSerializer {
    bool serialize(const CommandResponse &response, char *buffer, size_t capacity);
}
#endif
