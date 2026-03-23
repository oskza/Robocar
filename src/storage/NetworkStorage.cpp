#include "NetworkStorage.h"

void NetworkStorage::begin() { _prefs.begin("network_cfg", false); }

void NetworkStorage::reset() { _prefs.clear(); }

void NetworkStorage::loadConfig(NetworkConfig &target) {
    loadHostname(target.hostname, 32);
    loadLocalIP(target.localIP);
    loadGateway(target.gateway);
    loadSubnet(target.subnet);
    loadPrimaryDNS(target.primaryDNS);
    loadSecondaryDNS(target.secondaryDNS);
    loadSSID(target.ssid, 32);
    loadPassword(target.password, 64);
    target.intervalMs = loadIntervalMs();
}

void NetworkStorage::saveConfig(NetworkConfig &cfg) {
    saveHostname(cfg.hostname);
    saveLocalIP(cfg.localIP);
    saveGateway(cfg.gateway);
    saveSubnet(cfg.subnet);
    savePrimaryDNS(cfg.primaryDNS);
    saveSecondaryDNS(cfg.secondaryDNS);
    saveSSID(cfg.ssid);
    savePassword(cfg.password);
    saveIntervalMs(cfg.intervalMs);
}

void NetworkStorage::loadHostname(char *target, size_t len) {
    if (_prefs.getString("hostname", target, len) == 0) {
        strncpy(target, NetworkDefaults::hostname, len);
        target[len - 1] = '\0';
    }
}

void NetworkStorage::saveHostname(const char *hostname) { _prefs.putString("hostname", hostname); }

void NetworkStorage::loadSSID(char* target, size_t len) {
    if (_prefs.getString("ssid", target, len) == 0)
        target[0] = '\0';
}

void NetworkStorage::saveSSID(const char *ssid) { _prefs.putString("ssid", ssid); }

void NetworkStorage::loadPassword(char* target, size_t len) {
    if (_prefs.getString("password", target, len) == 0)
        target[0] = '\0';
}

void NetworkStorage::savePassword(const char *password) { _prefs.putString("password", password); }

void NetworkStorage::loadLocalIP(uint8_t *target) {
    if (_prefs.getBytes("local_ip", target, 4) != 4)
        memcpy(target, NetworkDefaults::localIP, 4);
}

void NetworkStorage::saveLocalIP(const uint8_t *ip) { _prefs.putBytes("local_ip", ip, 4); }

void NetworkStorage::loadGateway(uint8_t *target) {
    if (_prefs.getBytes("gateway", target, 4) != 4)
        memcpy(target, NetworkDefaults::gateway, 4);
}

void NetworkStorage::saveGateway(const uint8_t *gateway) { _prefs.putBytes("gateway", gateway, 4); }

void NetworkStorage::loadSubnet(uint8_t *target) {
    if (_prefs.getBytes("subnet", target, 4) != 4)
        memcpy(target, NetworkDefaults::subnet, 4);
}

void NetworkStorage::saveSubnet(const uint8_t *subnet) { _prefs.putBytes("subnet", subnet, 4); }

void NetworkStorage::loadPrimaryDNS(uint8_t *target) {
    if (_prefs.getBytes("dns1", target, 4) != 4)
        memcpy(target, NetworkDefaults::primaryDNS, 4);
}

void NetworkStorage::savePrimaryDNS(const uint8_t *dns) { _prefs.putBytes("dns1", dns, 4); }

void NetworkStorage::loadSecondaryDNS(uint8_t *target) {
    if (_prefs.getBytes("dns2", target, 4) != 4)
        memcpy(target, NetworkDefaults::secondaryDNS, 4);
}

void NetworkStorage::saveSecondaryDNS(const uint8_t *dns) { _prefs.putBytes("dns2", dns, 4); }

uint32_t NetworkStorage::loadIntervalMs() { return _prefs.getULong("interval", NetworkDefaults::intervalMs); }

void NetworkStorage::saveIntervalMs(uint32_t ms) { _prefs.putULong("interval", ms); }
