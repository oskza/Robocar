#ifndef SYSTEM_SNAPSHOT_H
#define SYSTEM_SNAPSHOT_H
#include <stdint.h>

struct SystemSnapshot {
    uint32_t uptimeMs;
    struct {
        uint32_t freeBytes;
        uint32_t minFreeBytes;
        uint32_t maxAllocBytes;
    } heap;
};
#endif
