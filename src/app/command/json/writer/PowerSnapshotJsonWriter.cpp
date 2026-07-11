#include "PowerSnapshotJsonWriter.h"

void PowerSnapshotJsonWriter::write(JsonObject json, const PowerSnapshot &snapshot) {
    json["connected"] = snapshot.connected;
    if (!snapshot.connected)
        return;
    json["hasPower"] = snapshot.hasPower;
    json["volts"] = snapshot.volts;
    if (!snapshot.hasPower)
        return;
    json["milliamps"] = snapshot.milliamps;
    json["milliwatts"] = snapshot.milliwatts;
}
