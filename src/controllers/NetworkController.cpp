#include "NetworkController.h"

NetworkController::NetworkController(NetworkStorage &storage, Timer &timer) 
                                        : _storage(storage), _timer(timer), _config{} {}

bool NetworkController::init() {
    _storage.begin();
    _storage.loadConfig(_config);
    _timer.setTimeout(_config.intervalMs);
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(_config.hostname);
    return WiFi.config(
        _config.localIP, 
        _config.gateway, 
        _config.subnet, 
        _config.primaryDNS, 
        _config.secondaryDNS
    );
}

bool NetworkController::tick() {
    if (!_timer.tick())
        return false;
    bool res = checkConnectivity();
    _timer.refresh();
    return res;
}

wl_status_t NetworkController::connect() {
    WiFi.begin(_config.ssid, _config.password);
    _timer.start();
    return WiFi.status();
}

bool NetworkController::checkConnectivity() { return isConnected() || WiFi.reconnect();}

bool NetworkController::isConnected() const { return WiFi.status() == WL_CONNECTED; }

int8_t NetworkController::getRSSI() const { return WiFi.RSSI(); }

void NetworkController::getConfig(NetworkConfig &target) const { _storage.loadConfig(target); }

/** TODO: apply */
void NetworkController::updateConfig(NetworkConfig &cfg) {
    updateHostname(cfg.hostname);
    updateSSID(cfg.ssid);
    updatePassword(cfg.password);
    updateLocalIP(cfg.localIP);
    updateGateway(cfg.gateway);
    updateSubnet(cfg.subnet);
    updatePrimaryDNS(cfg.primaryDNS);
    updateSecondaryDNS(cfg.secondaryDNS);
    updateIntervalMs(cfg.intervalMs);
}

/** TODO: ssid, password */
void NetworkController::resetConfig() {
    _storage.reset();
    updateHostname(NetworkDefaults::hostname);
    updateLocalIP(NetworkDefaults::localIP);
    updateGateway(NetworkDefaults::gateway);
    updateSubnet(NetworkDefaults::subnet);
    updatePrimaryDNS(NetworkDefaults::primaryDNS);
    updateSecondaryDNS(NetworkDefaults::secondaryDNS);
    updateIntervalMs(NetworkDefaults::intervalMs);
}

void NetworkController::updateHostname(const char *hostname) {
    if (strcmp(_config.hostname, hostname) == 0)
        return;
    strncpy(_config.hostname, hostname, 32);
    _config.hostname[31] = '\0';
    _storage.saveHostname(hostname);
}

void NetworkController::updateSSID(const char *ssid) {
    if (strcmp(_config.ssid, ssid) == 0)
        return;
    strncpy(_config.ssid, ssid, 32);
    _config.ssid[31] = '\0';
    _storage.saveSSID(ssid);
}

void NetworkController::updatePassword(const char *password) {
    if (strcmp(_config.password, password) == 0)
        return;
    strncpy(_config.password, password, 64);
    _config.password[63] = '\0';
    _storage.savePassword(password);
}

void NetworkController::updateLocalIP(const uint8_t *ip) {
    if (memcmp(_config.localIP, ip, 4) == 0)
        return;
    memcpy(_config.localIP, ip, 4);
    _storage.saveLocalIP(ip);
}

void NetworkController::updateGateway(const uint8_t *gateway) {
    if (memcmp(_config.gateway, gateway, 4) == 0)
        return;
    memcpy(_config.gateway, gateway, 4);
    _storage.saveGateway(gateway);
}

void NetworkController::updateSubnet(const uint8_t *subnet) {
    if (memcmp(_config.subnet, subnet, 4) == 0)
        return;
    memcpy(_config.subnet, subnet, 4);
    _storage.saveSubnet(subnet);
}

void NetworkController::updatePrimaryDNS(const uint8_t *dns) {
    if (memcmp(_config.primaryDNS, dns, 4) == 0)
        return;
    memcpy(_config.primaryDNS, dns, 4);
    _storage.savePrimaryDNS(dns);
}

void NetworkController::updateSecondaryDNS(const uint8_t *dns) {
    if (memcmp(_config.secondaryDNS, dns, 4) == 0)
        return;
    memcpy(_config.secondaryDNS, dns, 4);
    _storage.saveSecondaryDNS(dns);
}

void NetworkController::updateIntervalMs(uint32_t ms) {
    if (_timer.getTimeout() == ms)
        return;
    _timer.setTimeout(ms);
    _config.intervalMs = ms;
    _storage.saveIntervalMs(ms);
}
