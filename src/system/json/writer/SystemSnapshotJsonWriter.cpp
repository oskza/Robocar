#include "SystemSnapshotJsonWriter.h"

namespace SystemSnapshotJsonWriter {
    void write(JsonObject json, const SystemSnapshot &snapshot) {
        json["uptimeMs"] = snapshot.uptimeMs;
        JsonObject heap = json["heap"].to<JsonObject>();
        heap["freeBytes"] = snapshot.heap.freeBytes;
        heap["minFreeBytes"] = snapshot.heap.minFreeBytes;
        heap["maxAllocBytes"] = snapshot.heap.maxAllocBytes;
    }
}
