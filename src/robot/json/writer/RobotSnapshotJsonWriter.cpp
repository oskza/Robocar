#include "RobotSnapshotJsonWriter.h"
#include "motion/json/writer/MotionSnapshotJsonWriter.h"
#include "network/json/writer/WifiSnapshotJsonWriter.h"
#include "power/json/writer/PowerSnapshotJsonWriter.h"
#include "system/json/writer/SystemSnapshotJsonWriter.h"

namespace RobotSnapshotJsonWriter {
    void write(JsonObject json, const RobotSnapshot &snapshot) {
        SystemSnapshotJsonWriter::write(json["system"].to<JsonObject>(), snapshot.system);
        PowerSnapshotJsonWriter::write(json["power"].to<JsonObject>(), snapshot.power);
        WifiSnapshotJsonWriter::write(json["wifi"].to<JsonObject>(), snapshot.wifi);
        MotionSnapshotJsonWriter::write(json["motion"].to<JsonObject>(), snapshot.motion);
    }
}
