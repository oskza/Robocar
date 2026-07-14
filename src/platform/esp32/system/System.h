#ifndef PLATFORM_SYSTEM_H
#define PLATFORM_SYSTEM_H
#include "domain/system/SystemSnapshot.h"

namespace Platform::System {
    uint32_t getUptimeMs();
    [[noreturn]] void restart();
    SystemSnapshot getSnapshot();
}
#endif
