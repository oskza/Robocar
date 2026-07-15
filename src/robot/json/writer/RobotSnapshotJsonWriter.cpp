#include "RobotSnapshotJsonWriter.h"
#include "system/json/writer/SystemSnapshotJsonWriter.h"
#include "power/json/PowerSnapshotJsonWriter.h"
#include "motion/json/writer/MotionSnapshotJsonWriter.h"
#include "network/json/writer/WifiSnapshotJsonWriter.h"

void RobotSnapshotJsonWriter::write(JsonObject json, const RobotSnapshot &snapshot) {
    SystemSnapshotJsonWriter::write(json["system"].to<JsonObject>(), snapshot.system);
    PowerSnapshotJsonWriter::write(json["power"].to<JsonObject>(), snapshot.power);
    MotionSnapshotJsonWriter::write(json["motion"].to<JsonObject>(), snapshot.motion);
    WifiSnapshotJsonWriter::write(json["network"].to<JsonObject>(), snapshot.network);
}
