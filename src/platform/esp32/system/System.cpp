#include "System.h"
#include <Arduino.h>
#include <Esp.h>

namespace Platform::System {
    uint32_t getUptimeMs() { return millis(); }

    [[noreturn]] void restart() {
        ESP.restart();
        while (true) {}
    }

    SystemSnapshot getSnapshot() {
        SystemSnapshot snapshot{};
        snapshot.uptimeMs = getUptimeMs();
        snapshot.heap.freeBytes = ESP.getFreeHeap();
        snapshot.heap.minFreeBytes = ESP.getMinFreeHeap();
        snapshot.heap.maxAllocBytes = ESP.getMaxAllocHeap();
        return snapshot;
    }
}
