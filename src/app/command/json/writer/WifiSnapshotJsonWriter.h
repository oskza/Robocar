#ifndef WIFI_SNAPSHOT_JSON_WRITER_H
#define WIFI_SNAPSHOT_JSON_WRITER_H
#include <ArduinoJson.h>
#include "../../../../subsystems/network/WifiSnapshot.h"
#include "../../../../subsystems/network/WifiStationState.h"
#include "../../../../subsystems/network/WifiModeStrings.h"

class WifiSnapshotJsonWriter {
private:
    static const char *_stationStateToString(WifiStationState state);
    static const char *_accessPointStateToString(WifiAccessPointState state);
public:
    static void write(JsonObject json, const WifiSnapshot &snapshot);
};
#endif
