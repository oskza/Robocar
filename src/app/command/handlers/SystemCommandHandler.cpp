#include "SystemCommandHandler.h"
#include "../serialization/json/SystemCommandJsonDeserializer.h"

SystemCommandHandler::SystemCommandHandler(Executor executor)
    : _executor(executor) {}

bool SystemCommandHandler::handle(const char *data, size_t len) {
    if (!_executor)
        return false;

    SystemCommandReadResult result =
        SystemCommandJsonDeserializer::read(data, len);

    if (!result.ok)
        return false;

    _executor(result.command);
    return true;
}
