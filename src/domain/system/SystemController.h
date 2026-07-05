#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H
#include "SystemSnapshot.h"

class SystemController {
public:
    uint32_t getUptimeMs() const;
    SystemSnapshot getSnapshot() const;
    void restart();
    bool factoryReset();
};
#endif
