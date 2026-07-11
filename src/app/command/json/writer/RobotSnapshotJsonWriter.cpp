#include "RobotSnapshotJsonWriter.h"

#include "SystemSnapshotJsonWriter.h"
#include "PowerSnapshotJsonWriter.h"
#include "MotionSnapshotJsonWriter.h"
#include "WifiSnapshotJsonWriter.h"

void RobotSnapshotJsonWriter::write(JsonObject json, const RobotSnapshot &snapshot) {
    SystemSnapshotJsonWriter::write(json["system"].to<JsonObject>(), snapshot.system);
    PowerSnapshotJsonWriter::write(json["power"].to<JsonObject>(), snapshot.power);
    MotionSnapshotJsonWriter::write(json["motion"].to<JsonObject>(), snapshot.motion);
    WifiSnapshotJsonWriter::write(json["network"].to<JsonObject>(), snapshot.network);
}
