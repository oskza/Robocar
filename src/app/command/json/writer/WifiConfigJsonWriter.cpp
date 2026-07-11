#include "WifiConfigJsonWriter.h"
#include "../../../../subsystems/network/WifiModeStrings.h"

void WifiConfigJsonWriter::write(JsonObject json, const WifiConfig &config) {
    json["mode"] = WifiModeStrings::toString(config.mode);
    json["hostname"] = config.hostname;
    json["dhcp"] = config.dhcp;
    json["fallbackToAccessPoint"] = config.fallbackToAccessPoint;
    json["reconnectIntervalMs"] = config.reconnectIntervalMs;
    if (config.dhcp)
        return;
    JsonObject staticIp = json["staticIp"].to<JsonObject>();
    staticIp["ip"] = config.staticIp.ip.toString();
    staticIp["gateway"] = config.staticIp.gateway.toString();
    staticIp["subnet"] = config.staticIp.subnet.toString();
    staticIp["primaryDns"] = config.staticIp.primaryDns.toString();
    staticIp["secondaryDns"] = config.staticIp.secondaryDns.toString();
}
