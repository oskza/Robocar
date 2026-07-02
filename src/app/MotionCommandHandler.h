#ifndef MOTION_COMMAND_HANDLER_H
#define MOTION_COMMAND_HANDLER_H
#include <Arduino.h>
#include "../domain/motion/MotionCommand.h"

class MotionCommandHandler {
public:
    using Executor = void (*)(const MotionCommand &command);
    explicit MotionCommandHandler(Executor executor);
    bool handle(const char *data, size_t len);
private:
    Executor _executor;
};
#endif
