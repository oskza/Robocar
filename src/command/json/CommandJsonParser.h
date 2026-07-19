#ifndef COMMAND_JSON_PARSER_H
#define COMMAND_JSON_PARSER_H
#include <stddef.h>
#include "command/CommandEnvelope.h"
#include "command/CommandError.h"

namespace CommandJsonParser {
    CommandError parse(const uint8_t *data, size_t length, CommandEnvelope &command);
}
#endif
