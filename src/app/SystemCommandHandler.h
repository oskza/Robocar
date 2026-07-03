#ifndef SYSTEM_COMMAND_HANDLER_H
#define SYSTEM_COMMAND_HANDLER_H
#include <Arduino.h>
#include "../domain/system/SystemCommand.h"

class SystemCommandHandler {
public:
    using Executor = void (*)(const SystemCommand &command);
    explicit SystemCommandHandler(Executor executor);
    bool handle(const char *data, size_t len);
private:
    Executor _executor;
};
#endif
