#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include <stddef.h>
#include "CommandDispatcher.h"

class CommandProcessor {
private:
    CommandDispatcher &_dispatcher;
public:
    explicit CommandProcessor(CommandDispatcher &dispatcher);
    bool handle(
        const char *request,
        size_t requestLength,
        char *responseBuffer,
        size_t responseCapacity
    );
};
#endif
