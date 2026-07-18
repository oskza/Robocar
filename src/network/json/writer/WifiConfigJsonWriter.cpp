#include "WifiConfigJsonWriter.h"
#include "../../WifiModeStrings.h"

namespace WifiConfigJsonWriter {
    void write(JsonObject json, const WifiConfig &config) {
        json["mode"] = WifiModeStrings::toString(config.mode);
        json["hostname"] = config.hostname;
        json["accessPointFallback"] = config.accessPointFallback;
        json["maxReconnectAttempts"] = config.maxReconnectAttempts;
        json["reconnectIntervalMs"] = config.reconnectIntervalMs;
        json["stationConnectTimeoutMs"] = config.stationConnectTimeoutMs;
        json["dhcp"] = config.dhcp;
        if (config.dhcp)
            return;
        JsonObject staticIp = json["staticIp"].to<JsonObject>();
        staticIp["ip"] = config.staticIp.ip.toString();
        staticIp["gateway"] = config.staticIp.gateway.toString();
        staticIp["subnet"] = config.staticIp.subnet.toString();
        staticIp["primaryDns"] = config.staticIp.primaryDns.toString();
        staticIp["secondaryDns"] = config.staticIp.secondaryDns.toString();
    }
}
