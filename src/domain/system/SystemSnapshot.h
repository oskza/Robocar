#ifndef SYSTEM_SNAPSHOT_H
#define SYSTEM_SNAPSHOT_H
#include <stdint.h>

struct SystemSnapshot {
    uint32_t uptimeMs = 0;
    struct HeapSnapshot {
        uint32_t freeBytes = 0;
        uint32_t minFreeBytes = 0;
        uint32_t maxAllocBytes = 0;
    } heap;
};
#endif
