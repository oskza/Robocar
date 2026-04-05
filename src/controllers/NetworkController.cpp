#include "NetworkController.h"

NetworkController::NetworkController(NetworkStorage &storage, Timer &timer)
                                        : _storage(storage), _timer(timer) {}

void NetworkController::modeToString(uint8_t mode, char *target) {
    if (!target) 
        return;
    switch (mode) {
        case WIFI_STA:
            strcpy(target, "sta");
            break;
        case WIFI_AP:
            strcpy(target, "ap");
            break;
        case WIFI_AP_STA:
            strcpy(target, "apsta");
            break;
        default:
            strcpy(target, "off");
            break;
    }
}

bool NetworkController::_configureStation() {
    NetworkConfig cfg;
    _storage.loadConfig(cfg);
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(cfg.hostname);
    return (cfg.dhcp)
                ? WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE)
                : WiFi.config(cfg.localIP, cfg.gateway, cfg.subnet, 
                                cfg.primaryDNS, cfg.secondaryDNS);
}

void NetworkController::_setDHCP(bool enable) {
    _storage.saveDHCP(enable);
    if (WiFi.getMode() == WIFI_STA) {
        WiFi.disconnect(true);
        connect();
    }
}

bool NetworkController::init() {
    _storage.begin();
    _timer.setTimeout(_storage.loadIntervalMs());
    return hasCredentials() && _configureStation();
}

bool NetworkController::tick() {
    if (!_timer.tick())
        return false;
    _timer.refresh();
    return checkConnectivity();
}

wl_status_t NetworkController::connect() {
    WifiCredentials creds;
    _storage.loadCredentials(creds);
    if (creds.ssid[0] == '\0')
        return WL_NO_SSID_AVAIL;
    _timer.start();
    return (_configureStation()) ? WiFi.begin(creds.ssid, creds.password) : WL_CONNECT_FAILED;
}

void NetworkController::disconnect() {
    if (WiFi.getMode() == WIFI_AP) {
        WiFi.softAPdisconnect(true);
        return;
    }
    _timer.reset();
    WiFi.disconnect(true);
}

bool NetworkController::checkConnectivity() { return isConnected() || WiFi.reconnect();}

bool NetworkController::enableAP() {
    _timer.reset();
    WifiCredentials creds;
    _storage.loadAPCredentials(creds);
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(creds.ssid, creds.password);
}

void NetworkController::enableDHCP() { _setDHCP(true); }

void NetworkController::disableDHCP() { _setDHCP(false); }

bool NetworkController::isConnected() const { return WiFi.status() == WL_CONNECTED; }

bool NetworkController::hasCredentials() const {
    char ssid[32];
    _storage.loadSSID(ssid);
    return (ssid[0] != '\0');
}

void NetworkController::reset() { 
    _storage.clear();
    disconnect();
    enableAP();
}

void NetworkController::resetConfig() { 
    _storage.clearConfig();
    if (WiFi.getMode() == WIFI_STA) {
        WiFi.disconnect(true);
        connect();
    }
}

void NetworkController::resetCredentials() { 
    _storage.clearCredentials();
    if (WiFi.getMode() == WIFI_STA) {
        WiFi.disconnect(true);
        enableAP();
    }
}

void NetworkController::resetAPCredentials() { 
    _storage.clearAPCredentials();
    if (WiFi.getMode() == WIFI_AP) {
        WiFi.softAPdisconnect(true);
        enableAP();
    }
}

void NetworkController::getStatus(NetworkStatus &target) const {
    memset(&target, 0, sizeof(NetworkStatus));
    wifi_mode_t mode = WiFi.getMode();
    modeToString((uint8_t)mode, target.mode);
    if (mode == WIFI_AP) {
        target.ap.clients = WiFi.softAPgetStationNum();
        return;
    }
    bool connected = isConnected();
    target.sta.connected = connected;
    if (connected)
        target.sta.rssi = WiFi.RSSI();
}

void NetworkController::getConfig(NetworkConfig &target) const { _storage.loadConfig(target);  }

void NetworkController::getCredentials(WifiCredentials &creds) const { _storage.loadCredentials(creds); }

void NetworkController::getAPCredentials(WifiCredentials &creds) const { _storage.loadAPCredentials(creds); }

void NetworkController::updateConfig(const NetworkConfig &cfg) {
    _storage.saveConfig(cfg);
    if (WiFi.getMode() == WIFI_STA) {
        WiFi.disconnect(true);
        connect();
    }
}

void NetworkController::updateCredentials(const WifiCredentials &creds) {
    _storage.saveCredentials(creds);
    disconnect();
    connect();
}

void NetworkController::updateAPCredentials(const WifiCredentials &creds) { 
    _storage.saveAPCredentials(creds);
    if (WiFi.getMode() == WIFI_AP) {
        WiFi.softAPdisconnect(true);
        enableAP();
    }
}
