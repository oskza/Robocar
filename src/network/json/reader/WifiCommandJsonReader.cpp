#include "WifiCommandJsonReader.h"
#include <string.h>
#include "json/JsonValueReader.h"
#include "../../WifiModeStrings.h"

bool WifiCommandJsonReader::_readConfig(JsonObjectConst json, WifiConfig &cfg) {
    JsonVariantConst modeValue = json["mode"];
    if (!(modeValue.is<const char*>() && WifiModeStrings::fromString(modeValue.as<const char*>(), cfg.mode)))
        return false;
    if (!JsonValueReader::readCString(json["hostname"], cfg.hostname, sizeof(cfg.hostname)))
        return false;
    if (!JsonValueReader::readBool(json["fallbackToAccessPoint"], cfg.fallbackToAccessPoint))
        return false;
    if (!JsonValueReader::readUint32(json["reconnectIntervalMs"], cfg.reconnectIntervalMs))
        return false;
    if (!JsonValueReader::readBool(json["dhcp"], cfg.dhcp))
        return false;
    if (cfg.dhcp)
        return true;
    JsonVariantConst staticIp = json["staticIp"];
    return staticIp.is<JsonObjectConst>()
        && JsonValueReader::readIpAddress(staticIp["ip"], cfg.staticIp.ip)
        && JsonValueReader::readIpAddress(staticIp["gateway"], cfg.staticIp.gateway)
        && JsonValueReader::readIpAddress(staticIp["subnet"], cfg.staticIp.subnet)
        && JsonValueReader::readIpAddress(staticIp["primaryDns"], cfg.staticIp.primaryDns)
        && JsonValueReader::readIpAddress(staticIp["secondaryDns"], cfg.staticIp.secondaryDns);
}

bool WifiCommandJsonReader::_readCredentials(JsonObjectConst json, WifiCredentials &creds) {
    return JsonValueReader::readCString(json["ssid"], creds.ssid, sizeof(creds.ssid))
        && JsonValueReader::readCString(json["password"], creds.password, sizeof(creds.password));
}

bool WifiCommandJsonReader::read(const char *commandName, JsonObjectConst payload, CommandEnvelope &command) {
    if (commandName == nullptr)
        return false;
    command.domain = CommandDomain::WIFI;
    if (strcmp(commandName, "getConfig") == 0) {
        command.command.wifi = WifiCommand::GET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "setConfig") == 0) {
        command.command.wifi = WifiCommand::SET_CONFIG;
        WifiConfig &cfg = command.payload.wifi.cfg;
        return _readConfig(payload, cfg);
    }
    if (strcmp(commandName, "resetConfig") == 0) {
        command.command.wifi = WifiCommand::RESET_CONFIG;
        return true;
    }
    if (strcmp(commandName, "setStationCredentials") == 0) {
        command.command.wifi = WifiCommand::SET_STATION_CREDENTIALS;
        return _readCredentials(payload, command.payload.wifi.credentials);
    }
    if (strcmp(commandName, "resetStationCredentials") == 0) {
        command.command.wifi = WifiCommand::RESET_STATION_CREDENTIALS;
        return true;
    }
    if (strcmp(commandName, "getAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::GET_ACCESS_POINT_CREDENTIALS;
        return true;
    }
    if (strcmp(commandName, "setAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::SET_ACCESS_POINT_CREDENTIALS;
        return _readCredentials(payload, command.payload.wifi.credentials);
    }
    if (strcmp(commandName, "resetAccessPointCredentials") == 0) {
        command.command.wifi = WifiCommand::RESET_ACCESS_POINT_CREDENTIALS;
        return true;
    }
    if (strcmp(commandName, "reset") == 0) {
        command.command.wifi = WifiCommand::RESET_ALL;
        return true;
    }
    return false;
}
