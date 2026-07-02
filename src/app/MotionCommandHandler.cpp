#include "MotionCommandHandler.h"
#include "../serialization/json/MotionCommandJsonDeserializer.h"

MotionCommandHandler::MotionCommandHandler(Executor executor) : _executor(executor) {}

bool MotionCommandHandler::handle(const char *data, size_t len) {
    if (!_executor)
        return false;
    MotionCommandReadResult result = MotionCommandJsonDeserializer::read(data, len);
    if (!result.ok)
        return false;
    _executor(result.command);
    return true;
}
