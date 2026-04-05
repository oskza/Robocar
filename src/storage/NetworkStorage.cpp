#include "NetworkStorage.h"

#define PREFS_NETWORK_CFG       "net_cfg"
#define PREFS_DHCP_KEY          "dchp"
#define PREFS_INTERVAL_KEY      "interval"
#define PREFS_HOSTNAME_KEY      "hostname"
#define PREFS_LOCAL_IP_KEY      "local_ip"
#define PREFS_GATEWAY_KEY       "gateway"
#define PREFS_SUBNET_KEY        "subnet"
#define PREFS_DNS_1_KEY         "dns1"
#define PREFS_DNS_2_KEY         "dns2"
#define PREFS_SSID_KEY          "ssid"
#define PREFS_PASSWORD_KEY      "password"
#define PREFS_AP_SSID_KEY       "ap_ssid"
#define PREFS_AP_PASSWORD_KEY   "ap_password"

void NetworkStorage::begin() { _prefs.begin(PREFS_NETWORK_CFG, false); }

void NetworkStorage::clear() { _prefs.clear(); }

void NetworkStorage::clearConfig() {
    _prefs.remove(PREFS_DHCP_KEY);
    _prefs.remove(PREFS_INTERVAL_KEY);
    _prefs.remove(PREFS_HOSTNAME_KEY);
    _prefs.remove(PREFS_LOCAL_IP_KEY);
    _prefs.remove(PREFS_GATEWAY_KEY);
    _prefs.remove(PREFS_SUBNET_KEY);
    _prefs.remove(PREFS_DNS_1_KEY);
    _prefs.remove(PREFS_DNS_2_KEY);
}

void NetworkStorage::clearCredentials() {
    _prefs.remove(PREFS_SSID_KEY);
    _prefs.remove(PREFS_PASSWORD_KEY);
}

void NetworkStorage::clearAPCredentials() {
    _prefs.remove(PREFS_AP_SSID_KEY);
    _prefs.remove(PREFS_AP_PASSWORD_KEY);
}

void NetworkStorage::loadConfig(NetworkConfig &target) {
    target.dhcp = loadDHCP();
    target.intervalMs = loadIntervalMs();
    loadHostname(target.hostname);
    loadLocalIP(target.localIP);
    loadGateway(target.gateway);
    loadSubnet(target.subnet);
    loadPrimaryDNS(target.primaryDNS);
    loadSecondaryDNS(target.secondaryDNS);
}

void NetworkStorage::loadCredentials(WifiCredentials &target) {
    loadSSID(target.ssid);
    loadPassword(target.password);
}

void NetworkStorage::loadAPCredentials(WifiCredentials &target) {
    loadAPSSID(target.ssid);
    loadAPPassword(target.password);
}

bool NetworkStorage::loadDHCP() { return _prefs.getBool(PREFS_DHCP_KEY, NetworkDefaults::dhcp); }

uint32_t NetworkStorage::loadIntervalMs() { return _prefs.getULong(PREFS_INTERVAL_KEY, NetworkDefaults::intervalMs); }

void NetworkStorage::loadHostname(char *target) {
    if (_prefs.getString(PREFS_HOSTNAME_KEY, target, 32) == 0) {
        strncpy(target, NetworkDefaults::hostname, 32);
        target[31] = '\0';
    }
}

void NetworkStorage::loadLocalIP(uint8_t *target) {
    if (_prefs.getBytes(PREFS_LOCAL_IP_KEY, target, 4) != 4)
        memcpy(target, NetworkDefaults::localIP, 4);
}

void NetworkStorage::loadGateway(uint8_t *target) {
    if (_prefs.getBytes(PREFS_GATEWAY_KEY, target, 4) != 4)
        memcpy(target, NetworkDefaults::gateway, 4);
}

void NetworkStorage::loadSubnet(uint8_t *target) {
    if (_prefs.getBytes(PREFS_SUBNET_KEY, target, 4) != 4)
        memcpy(target, NetworkDefaults::subnet, 4);
}

void NetworkStorage::loadPrimaryDNS(uint8_t *target) {
    if (_prefs.getBytes(PREFS_DNS_1_KEY, target, 4) != 4)
        memcpy(target, NetworkDefaults::primaryDNS, 4);
}

void NetworkStorage::loadSecondaryDNS(uint8_t *target) {
    if (_prefs.getBytes(PREFS_DNS_2_KEY, target, 4) != 4)
        memcpy(target, NetworkDefaults::secondaryDNS, 4);
}

void NetworkStorage::loadSSID(char* target) {
    if (_prefs.getString(PREFS_SSID_KEY, target, 32) == 0)
        target[0] = '\0';
}

void NetworkStorage::loadPassword(char* target) {
    if (_prefs.getString(PREFS_PASSWORD_KEY, target, 64) == 0)
        target[0] = '\0';
}

void NetworkStorage::loadAPSSID(char *target) {
    if (_prefs.getString(PREFS_AP_SSID_KEY, target, 32) == 0) {
        strncpy(target, NetworkDefaults::apSSID, 32);
        target[31] = '\0';
    }
}

void NetworkStorage::loadAPPassword(char *target) {
    if (_prefs.getString(PREFS_AP_PASSWORD_KEY, target, 64) == 0) {
        strncpy(target, NetworkDefaults::apPassword, 64);
        target[63] = '\0';
    }
}

void NetworkStorage::saveConfig(const NetworkConfig &cfg) {
    saveDHCP(cfg.dhcp);
    saveIntervalMs(cfg.intervalMs);
    saveHostname(cfg.hostname);
    saveLocalIP(cfg.localIP);
    saveGateway(cfg.gateway);
    saveSubnet(cfg.subnet);
    savePrimaryDNS(cfg.primaryDNS);
    saveSecondaryDNS(cfg.secondaryDNS);
}

void NetworkStorage::saveCredentials(const WifiCredentials &creds) {
    saveSSID(creds.ssid);
    savePassword(creds.password);
}

void NetworkStorage::saveAPCredentials(const WifiCredentials &creds) {
    saveAPSSID(creds.ssid);
    saveAPPassword(creds.password);
}

void NetworkStorage::saveDHCP(bool dhcp) { _prefs.putBool(PREFS_DHCP_KEY, dhcp); }

void NetworkStorage::saveIntervalMs(uint32_t ms) { _prefs.putULong(PREFS_INTERVAL_KEY, ms); }

void NetworkStorage::saveHostname(const char *hostname) { _prefs.putString(PREFS_HOSTNAME_KEY, hostname); }

void NetworkStorage::saveLocalIP(const uint8_t *ip) { _prefs.putBytes(PREFS_LOCAL_IP_KEY, ip, 4); }

void NetworkStorage::saveGateway(const uint8_t *gateway) { _prefs.putBytes(PREFS_GATEWAY_KEY, gateway, 4); }

void NetworkStorage::saveSubnet(const uint8_t *subnet) { _prefs.putBytes(PREFS_SUBNET_KEY, subnet, 4); }

void NetworkStorage::savePrimaryDNS(const uint8_t *dns) { _prefs.putBytes(PREFS_DNS_1_KEY, dns, 4); }

void NetworkStorage::saveSecondaryDNS(const uint8_t *dns) { _prefs.putBytes(PREFS_DNS_2_KEY, dns, 4); }

void NetworkStorage::saveSSID(const char *ssid) { _prefs.putString(PREFS_SSID_KEY, ssid); }

void NetworkStorage::savePassword(const char *password) { _prefs.putString(PREFS_PASSWORD_KEY, password); }

void NetworkStorage::saveAPSSID(const char *ssid) { _prefs.putString(PREFS_AP_SSID_KEY, ssid); }

void NetworkStorage::saveAPPassword(const char *password) { _prefs.putString(PREFS_AP_PASSWORD_KEY, password); }
