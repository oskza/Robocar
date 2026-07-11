#ifndef COMMAND_SERVICE_H
#define COMMAND_SERVICE_H
#include "CommandDispatcher.h"

class Robot;

class CommandService {
private:
    CommandDispatcher _dispatcher;
public:
    explicit CommandService(Robot &robot);
    bool handle(
        const char *request,
        size_t requestLength,
        char *response,
        size_t responseCapacity
    );
};
#endif
