#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H
#include <stdint.h>

enum class SystemCommand : uint8_t { STATUS, PING, RESTART, FACTORY_RESET };

struct SystemCommandPayload {};
#endif
