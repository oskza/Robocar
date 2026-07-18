#include "WifiCommandJsonReader.h"
#include <string.h>
#include <JsonValueReader.h>
#include "../../WifiModeStrings.h"

namespace {
    bool readStaticIpConfig(JsonObjectConst json, WifiStaticIpConfig &config) {
        return JsonValueReader::readIpAddress(json["ip"], config.ip)
            && JsonValueReader::readIpAddress(json["gateway"], config.gateway)
            && JsonValueReader::readIpAddress(json["subnet"], config.subnet)
            && JsonValueReader::readIpAddress(json["primaryDns"], config.primaryDns)
            && JsonValueReader::readIpAddress(json["secondaryDns"], config.secondaryDns);
    }

    bool readConfig(JsonObjectConst json, WifiConfig &config) {
        const JsonVariantConst modeValue = json["mode"];
        if (!modeValue.is<const char *>())
            return false;
        if (!WifiModeStrings::fromString(modeValue.as<const char*>(), config.mode))
            return false;
        if (!JsonValueReader::readCString(json["hostname"], config.hostname, sizeof(config.hostname)))
            return false;
        if (!JsonValueReader::readBool(json["accessPointFallback"], config.accessPointFallback))
            return false;
        if (!JsonValueReader::readUint32(json["maxReconnectAttempts"], config.maxReconnectAttempts))
            return false;
        if (!JsonValueReader::readUint32(json["reconnectIntervalMs"], config.reconnectIntervalMs))
            return false;
        if (!JsonValueReader::readUint32(json["stationConnectTimeoutMs"], config.stationConnectTimeoutMs))
            return false;
        if (!JsonValueReader::readBool(json["dhcp"], config.dhcp))
            return false;
        if (config.dhcp) {
            config.staticIp = {};
            return true;
        }
        const JsonVariantConst staticIpValue = json["staticIp"];
        return (staticIpValue.is<JsonObjectConst>())
            ? readStaticIpConfig(staticIpValue.as<JsonObjectConst>(), config.staticIp)
            : false;
    }

    bool readCredentials(JsonObjectConst json, WifiCredentials &credentials) {
        return JsonValueReader::readCString(json["ssid"], credentials.ssid, sizeof(credentials.ssid))
            && JsonValueReader::readCString(json["password"], credentials.password, sizeof(credentials.password));
    }
}

namespace WifiCommandJsonReader {
    bool read(const char *command, JsonObjectConst payload, WifiCommand &out) {
        if (command == nullptr)
            return false;
        out.type = WifiCommandType::UNKNOWN;
        if (strcmp(command, "getConfig") == 0) {
            out.type = WifiCommandType::GET_CONFIG;
            return true;
        }
        if (strcmp(command, "setConfig") == 0) {
            out.type = WifiCommandType::SET_CONFIG;
            return readConfig(payload, out.payload.config);
        }
        if (strcmp(command, "resetConfig") == 0) {
            out.type = WifiCommandType::RESET_CONFIG;
            return true;
        }
        if (strcmp(command, "setStationCredentials") == 0) {
            out.type = WifiCommandType::SET_STATION_CREDENTIALS;
            return readCredentials(payload, out.payload.credentials);
        }
        if (strcmp(command, "resetStationCredentials") == 0) {
            out.type = WifiCommandType::RESET_STATION_CREDENTIALS;
            return true;
        }
        if (strcmp(command, "getAccessPointCredentials") == 0) {
            out.type = WifiCommandType::GET_ACCESS_POINT_CREDENTIALS;
            return true;
        }
        if (strcmp(command, "setAccessPointCredentials") == 0) {
            out.type = WifiCommandType::SET_ACCESS_POINT_CREDENTIALS;
            return readCredentials(payload, out.payload.credentials);
        }
        if (strcmp(command, "resetAccessPointCredentials") == 0) {
            out.type =
                WifiCommandType::RESET_ACCESS_POINT_CREDENTIALS;
            return true;
        }
        if (strcmp(command, "reset") == 0) {
            out.type = WifiCommandType::RESET_ALL;
            return true;
        }
        return false;
    }
}
