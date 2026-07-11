#include "PowerSnapshotJsonWriter.h"

void PowerSnapshotJsonWriter::write(JsonObject json, const PowerSnapshot &snapshot) {
    json["connected"] = snapshot.connected;
    json["busVoltage"] = snapshot.busVoltage;
    json["currentMilliamps"] = snapshot.currentMilliamps;
    json["powerMilliwatts"] = snapshot.powerMilliwatts;
}
