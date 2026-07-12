#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include "CommandDispatcher.h"

class CommandProcessor {
private:
    CommandDispatcher _dispatcher;
public:
    explicit CommandProcessor(Robot &robot);
    bool handle(
        const char *request,
        size_t requestLength,
        char *response,
        size_t responseCapacity
    );
};
#endif
