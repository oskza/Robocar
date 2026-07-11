#ifndef COMMAND_JSON_SERIALIZER_H
#define COMMAND_JSON_SERIALIZER_H

#include <stddef.h>
#include "../common/CommandResponse.h"

class CommandJsonSerializer {
private:
    static const char *_statusToString(CommandResponseStatus status);
    static const char *_typeToString(CommandResponseType type);
    static const char *_errorToString(CommandError error);

public:
    static bool serialize(
        const CommandResponse &response,
        char *buffer,
        size_t size
    );
};

#endif
