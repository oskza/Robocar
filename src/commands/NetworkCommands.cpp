#include "NetworkCommands.h"
#include "controllers/NetworkController.h"

extern NetworkController networkController;

static bool parseIP(uint8_t target[4], JsonVariant value) {
    IPAddress ip;
    const char* str = value | "";
    if (!ip.fromString(str)) 
        return false;
    for (int i = 0; i < 4; i++)
        target[i] = ip[i];
    return true;
}

static void handleGetConfig(JsonDocument &doc) {
    NetworkConfig cfg;
    JsonObject json = doc["cfg"].to<JsonObject>();
    networkController.getConfig(cfg);
    json["hostname"] = cfg.hostname;
    json["ip"] = cfg.localIP;
    json["gateway"] = cfg.gateway;
    json["subnet"] = cfg.subnet;
    json["dns1"] = cfg.primaryDNS;
    json["dns2"] = cfg.secondaryDNS;
    json["interval"] = cfg.intervalMs;
}

static void handleGetWifiCredentials(JsonDocument &doc) {
    JsonObject json = doc["creds"];
    WifiCredentials creds;
    networkController.getCredentials(creds);
    json["ssid"] = creds.ssid;
    json["password"] = creds.password;
}

static void handleGetAPCredentials(JsonDocument &doc) {
    JsonObject json = doc["creds"];
    WifiCredentials creds;
    networkController.getAPCredentials(creds);
    json["ssid"] = creds.ssid;
    json["password"] = creds.password;
}

static void handleUpdateConfig(JsonDocument &doc) {
    JsonObject payload = doc["payload"];
    NetworkConfig cfg;
    strncpy(cfg.hostname, payload["hostname"], 32);
    cfg.hostname[31] = '\0';
    parseIP(cfg.localIP, payload["ip"]);
    parseIP(cfg.gateway, payload["gateway"]);
    parseIP(cfg.subnet, payload["subnet"]);
    parseIP(cfg.primaryDNS, payload["dns1"]);
    parseIP(cfg.secondaryDNS, payload["dns2"]);
    cfg.intervalMs = payload["interval"];
    networkController.updateConfig(cfg);
}

static void handleUpdateWifiCredentials(JsonDocument &doc) {
    JsonObject payload = doc["payload"];
    WifiCredentials creds;
    strncpy(creds.ssid, payload["ssid"], 32);
    creds.ssid[31] = '\0';
    strncpy(creds.password, payload["password"], 64);
    creds.password[63] = '\0';
    networkController.updateCredentials(creds);
}

static void handleUpdateAPCredentials(JsonDocument &doc) {
    JsonObject payload = doc["payload"];
    WifiCredentials creds;
    strncpy(creds.ssid, payload["ssid"], 32);
    creds.ssid[31] = '\0';
    strncpy(creds.password, payload["password"], 64);
    creds.password[63] = '\0';
    networkController.updateAPCredentials(creds);
}

CommandEntry networkCommands[] = {
    { "dhcp-on", [](JsonDocument &) { networkController.enableDHCP(); } },
    { "dhcp-off", [](JsonDocument &) { networkController.disableDHCP(); } },
    { "get-config", handleGetConfig },
    { "get-wifi-creds", handleGetWifiCredentials },
    { "get-ap-creds", handleGetAPCredentials },
    { "update-config", handleUpdateConfig },
    { "update-wifi-creds", handleUpdateWifiCredentials },
    { "update-ap-creds", handleUpdateAPCredentials },
    { "reset-config", [](JsonDocument &) { networkController.resetConfig(); } },
    { "reset-wifi-creds", [](JsonDocument &) { networkController.resetCredentials(); } },
    { "reset-ap-creds", [](JsonDocument &) { networkController.resetAPCredentials(); } },
    { "reset", [](JsonDocument &) { networkController.reset(); } }
};

const size_t networkCommandCount = sizeof(networkCommands) / sizeof(CommandEntry);
