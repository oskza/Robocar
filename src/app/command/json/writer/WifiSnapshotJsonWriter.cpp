#include "WifiSnapshotJsonWriter.h"

const char* WifiSnapshotJsonWriter::_stationStateToString(WifiStationState state) {
    switch (state) {
        case WifiStationState::OFF: return "off";
        case WifiStationState::DISCONNECTED: return "disconnected";
        case WifiStationState::CONNECTING: return "connecting";
        case WifiStationState::CONNECTED: return "connected";
    }
    return "unknown";
}

const char* WifiSnapshotJsonWriter::_accessPointStateToString(WifiAccessPointState state) {
    switch (state) {
        case WifiAccessPointState::OFF: return "off";
        case WifiAccessPointState::ACTIVE: return "active";
    }
    return "unknown";
}

void WifiSnapshotJsonWriter::write(JsonObject json, const WifiSnapshot &snapshot) {
    json["mode"] = WifiModeStrings::toString(snapshot.mode);
    json["hostname"] = snapshot.hostname;
    JsonObject station = json["station"].to<JsonObject>();
    station["state"] = _stationStateToString(snapshot.station.state);
    station["connected"] = snapshot.station.state == WifiStationState::CONNECTED;
    station["reconnectAttempts"] = snapshot.station.reconnectAttempts;
    if (snapshot.station.state == WifiStationState::CONNECTED) {
        station["ip"] = snapshot.station.ip.toString();
        station["ssid"] = snapshot.station.ssid;
        station["rssi"] = snapshot.station.rssi;
    }
    JsonObject accessPoint = json["accessPoint"].to<JsonObject>();
    accessPoint["state"] = _accessPointStateToString(snapshot.accessPoint.state);
    accessPoint["active"] = snapshot.accessPoint.state == WifiAccessPointState::ACTIVE;
    if (snapshot.accessPoint.state == WifiAccessPointState::ACTIVE) {
        accessPoint["localIp"] = snapshot.accessPoint.ip.toString();
        accessPoint["ssid"] = snapshot.accessPoint.ssid;
        accessPoint["clients"] = snapshot.accessPoint.clients;
    }
}
