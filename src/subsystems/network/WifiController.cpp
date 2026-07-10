#include "WifiController.h"
#include <cstring>

WifiController *WifiController::_instance = nullptr;

WifiController::WifiController()
    : _cfg{},
      _stationCredentials{},
      _accessPointCredentials{},
      _mode(WifiMode::OFF),
      _state(WifiState::OFF),
      _lastReconnectMs(0),
      _reconnectAttempts(0) { _instance = this; }

void WifiController::_onWifiEvent(WiFiEvent_t event) {
    if (_instance != nullptr) _instance->_handleWifiEvent(event);
}

void WifiController::_handleWifiEvent(WiFiEvent_t event) {
    if (_mode != WifiMode::STA && _mode != WifiMode::APSTA)
        return;
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            _state = WifiState::CONNECTING;
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            _state = WifiState::CONNECTED;
            _reconnectAttempts = 0;
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            _state = WifiState::DISCONNECTED;
            break;
        default:
            break;
    }
}

bool WifiController::_hasStationCredentials() const { return _stationCredentials.ssid[0] != '\0'; }

bool WifiController::_hasAccessPointCredentials() const { return _accessPointCredentials.ssid[0] != '\0'; }

void WifiController::_applyConfiguration() {
    if (!_startStation())
        _startAccessPoint();
}

void WifiController::_applyHostname() {
    if (_cfg.hostname[0] != '\0')
        WiFi.setHostname(_cfg.hostname);
}

void WifiController::_applyIpConfig() {
    if (_cfg.dhcp) {
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
        return;
    }
    WiFi.config(
        _cfg.staticIp.ip,
        _cfg.staticIp.gateway,
        _cfg.staticIp.subnet,
        _cfg.staticIp.primaryDns,
        _cfg.staticIp.secondaryDns
    );
}

bool WifiController::_startStation() {
    if (!_hasStationCredentials()) {
        return false;
    }

    _stopAccessPoint();

    WiFi.mode(WIFI_STA);

    _applyHostname();
    _applyIpConfig();

    WiFi.begin(
        _stationCredentials.ssid,
        _stationCredentials.password
    );

    _mode = WifiMode::STA;
    _state = WifiState::CONNECTING;

    return true;
}

bool WifiController::_startAccessPoint() {
    if (!_hasAccessPointCredentials()) {
        disconnect();
        return false;
    }

    _stopStation();

    WiFi.mode(WIFI_AP);

    const bool ok = WiFi.softAP(
        _accessPointCredentials.ssid,
        _accessPointCredentials.password
    );

    if (!ok) {
        _mode = WifiMode::OFF;
        _state = WifiState::OFF;
        return false;
    }

    _mode = WifiMode::AP;
    _state = WifiState::AP_ACTIVE;

    return true;
}

void WifiController::_stopStation() {
    if (_mode == WifiMode::STA || _mode == WifiMode::APSTA) {
        WiFi.disconnect(true);
    }
}

void WifiController::_stopAccessPoint() {
    if (_mode == WifiMode::AP || _mode == WifiMode::APSTA) {
        WiFi.softAPdisconnect(true);
    }
}

void WifiController::begin(
    const WifiConfig &cfg,
    const WifiCredentials &stationCredentials,
    const WifiCredentials &accessPointCredentials
) {
    _cfg = cfg;
    _stationCredentials = stationCredentials;
    _accessPointCredentials = accessPointCredentials;

    _lastReconnectMs = 0;
    _reconnectAttempts = 0;

    WiFi.onEvent(_onWifiEvent);

    _applyConfiguration();
}

void WifiController::update(uint32_t nowMs) {

    if (_mode != WifiMode::STA)
        return;

    if (_state != WifiState::DISCONNECTED)
        return;

    if (_cfg.reconnectIntervalMs == 0
            || nowMs - _lastReconnectMs < _cfg.reconnectIntervalMs)
        return;

    _lastReconnectMs = nowMs;
    _reconnectAttempts++;

    _startStation();
}

void WifiController::setConfig(const WifiConfig &cfg) {
    _cfg = cfg;
    if (_mode == WifiMode::STA) {
        _applyHostname();
        _applyIpConfig();
    }
}

void WifiController::setStationCredentials(const WifiCredentials &credentials) {
    _stationCredentials = credentials;
    _applyConfiguration();
}

void WifiController::setAccessPointCredentials(const WifiCredentials &credentials) {
    _accessPointCredentials = credentials;
    if (_mode == WifiMode::AP)
        _applyConfiguration();
}

void WifiController::disconnect() {
    _stopStation();
    _stopAccessPoint();

    WiFi.mode(WIFI_OFF);

    _mode = WifiMode::OFF;
    _state = WifiState::OFF;
    _lastReconnectMs = 0;
    _reconnectAttempts = 0;
}

bool WifiController::isConnected() const { return _state == WifiState::CONNECTED; }

bool WifiController::isAccessPointActive() const {
    return _mode == WifiMode::AP && _state == WifiState::AP_ACTIVE;
}

const char *WifiController::getHostname() const { return _cfg.hostname; }

WifiSnapshot WifiController::getSnapshot() const {
    WifiSnapshot snapshot{};

    snapshot.mode = _mode;
    snapshot.state = _state;

    snapshot.connected = isConnected();
    snapshot.accessPointActive = isAccessPointActive();

    snapshot.rssi = snapshot.connected ? WiFi.RSSI() : 0;

    snapshot.localIp = WiFi.localIP();
    snapshot.accessPointIp = WiFi.softAPIP();

    strncpy(snapshot.ssid, _stationCredentials.ssid, sizeof(snapshot.ssid));
    snapshot.ssid[sizeof(snapshot.ssid) - 1] = '\0';

    strncpy(snapshot.hostname, _cfg.hostname, sizeof(snapshot.hostname));
    snapshot.hostname[sizeof(snapshot.hostname) - 1] = '\0';

    snapshot.reconnectAttempts = _reconnectAttempts;

    return snapshot;
}
