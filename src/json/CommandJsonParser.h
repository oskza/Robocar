#ifndef COMMAND_JSON_PARSER_H
#define COMMAND_JSON_PARSER_H
#include <stddef.h>
#include "../command/CommandEnvelope.h"

class CommandJsonParser {
public:
    static bool parse(const char *data, size_t length, CommandEnvelope &command);
};
#endif
