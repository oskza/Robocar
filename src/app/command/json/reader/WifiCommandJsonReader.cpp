#include "WifiCommandJsonReader.h"
#include <string.h>
#include "../JsonValueReader.h"
#include "../../../../subsystems/network/WifiModeStrings.h"

bool WifiCommandJsonReader::_readConfig(JsonVariantConst &payload, WifiConfig &cfg) {
    JsonVariantConst modeValue = payload["mode"];
    if (!(modeValue.is<const char *>() && WifiModeStrings::fromString(modeValue.as<const char *>(), cfg.mode)))
        return false;
    if (!JsonValueReader::readCString(payload["hostname"], cfg.hostname, sizeof(cfg.hostname)))
        return false;
    if (!JsonValueReader::readBool(payload["fallbackToAccessPoint"], cfg.fallbackToAccessPoint))
        return false;
    if (!JsonValueReader::readUint32(payload["reconnectIntervalMs"], cfg.reconnectIntervalMs))
        return false;
    if (!JsonValueReader::readBool(payload["dhcp"], cfg.dhcp))
        return false;
    if (cfg.dhcp)
        return true;
    JsonVariantConst staticIp = payload["staticIp"];
    return staticIp.is<JsonObjectConst>()
        && JsonValueReader::readIpAddress(staticIp["ip"], cfg.staticIp.ip)
        && JsonValueReader::readIpAddress(staticIp["gateway"], cfg.staticIp.gateway)
        && JsonValueReader::readIpAddress(staticIp["subnet"], cfg.staticIp.subnet)
        && JsonValueReader::readIpAddress(staticIp["primaryDns"], cfg.staticIp.primaryDns)
        && JsonValueReader::readIpAddress(staticIp["secondaryDns"], cfg.staticIp.secondaryDns);
}

bool WifiCommandJsonReader::_readCredentials(JsonVariantConst &payload, WifiCredentials &creds) {
    return JsonValueReader::readCString(payload["ssid"], creds.ssid, sizeof(creds.ssid))
        && JsonValueReader::readCString(payload["password"], creds.password, sizeof(creds.password));
}

bool WifiCommandJsonReader::read(JsonObjectConst json, CommandEnvelope &command) {
    JsonVariantConst commandValue = json["command"];
    if (!commandValue.is<const char *>())
        return false;
    const char *cmd = commandValue.as<const char *>();
    command.domain = CommandDomain::WIFI;
    JsonVariantConst payload = json["payload"];
    if (strcmp(cmd, "status") == 0) {
        command.command.wifi = WifiCommand::STATUS;
        return true;
    }
    if (strcmp(cmd, "getConfig") == 0) {
        command.command.wifi = WifiCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(cmd, "setConfig") == 0) {
        command.command.wifi = WifiCommand::SET_CONFIG;
        WifiConfig &cfg = command.payload.wifi.cfg;
        return _readConfig(payload, cfg);
    }
    if (strcmp(cmd, "resetConfig") == 0) {
        command.command.wifi = WifiCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(cmd, "setStationCredentials") == 0) {
        command.command.wifi = WifiCommand::SET_STATION_CREDENTIALS;
        return _readCredentials(payload, command.payload.wifi.credentials);
    }
    if (strcmp(cmd, "resetStationCredentials") == 0) {
        command.command.wifi = WifiCommand::RESET_STATION_CREDENTIALS;
        return true;
    }
    if (strcmp(cmd, "getAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::GET_ACCESS_POINT_CREDENTIALS;
        return true;
    }
    if (strcmp(cmd, "setAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::SET_ACCESS_POINT_CREDENTIALS;
        return _readCredentials(payload, command.payload.wifi.credentials);
    }
    if (strcmp(cmd, "resetAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::RESET_ACCESS_POINT_CREDENTIALS;
        return true;
    }
    if (strcmp(cmd, "reset") == 0) {
        command.command.wifi = WifiCommand::RESET_ALL;
        return true;
    }
    return false;
}
