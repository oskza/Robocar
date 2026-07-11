#include "WifiController.h"
#include <cstring>

WifiController *WifiController::_instance = nullptr;

WifiController::WifiController()
    : _cfg{},
      _stationCredentials{},
      _accessPointCredentials{},
      _mode(WifiMode::OFF),
      _stationState(WifiStationState::OFF),
      _accessPointState(WifiAccessPointState::OFF),
      _lastReconnectMs(0),
      _reconnectAttempts(0) { _instance = this; }

void WifiController::_onWifiEvent(WiFiEvent_t event) {
    if (_instance != nullptr) _instance->_handleWifiEvent(event);
}

void WifiController::_handleWifiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_START:
            _stationState = WifiStationState::CONNECTING;
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            _stationState = WifiStationState::CONNECTING;
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            _stationState = WifiStationState::CONNECTED;
            _reconnectAttempts = 0;
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            if (_mode == WifiMode::STA || _mode == WifiMode::APSTA) {
                _stationState = WifiStationState::DISCONNECTED;
                break;
            }
            _stationState = WifiStationState::OFF;
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            _accessPointState = WifiAccessPointState::ACTIVE;
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            _accessPointState = WifiAccessPointState::OFF;
            break;
    }
}

bool WifiController::_hasStationCredentials() const { return _stationCredentials.ssid[0] != '\0'; }

bool WifiController::_hasAccessPointCredentials() const { return _accessPointCredentials.ssid[0] != '\0'; }

bool WifiController::_applyMode(WifiMode mode) {
    switch (mode) {
        case WifiMode::OFF:
            disconnect();
            return true;
        case WifiMode::STA:
            _stopAccessPoint();
            WiFi.mode(WIFI_STA);
            _mode = WifiMode::STA;
            return _startStation();
        case WifiMode::AP:
            _stopStation();
            WiFi.mode(WIFI_AP);
            _mode = WifiMode::AP;
            return _startAccessPoint();
        case WifiMode::APSTA:
            WiFi.mode(WIFI_AP_STA);
            _mode = WifiMode::APSTA;
            {
                const bool apStarted = _startAccessPoint();
                const bool stationStarted = _startStation();
                return apStarted || stationStarted;
            }
    }
    return false;
}

void WifiController::_applyConfiguration() {
    if (_applyMode(_cfg.mode))
        return;
    if (!_cfg.fallbackToAccessPoint) {
        disconnect();
        return;
    }
    WiFi.mode(WIFI_AP);
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
        _stationState = WifiStationState::OFF;
        return false;
    }
    _applyHostname();
    _applyIpConfig();
    WiFi.begin(
        _stationCredentials.ssid,
        _stationCredentials.password
    );
    _stationState = WifiStationState::CONNECTING;
    return true;
}

bool WifiController::_startAccessPoint() {
    if (!_hasAccessPointCredentials()) {
        _accessPointState = WifiAccessPointState::OFF;
        return false;
    }
    if (!WiFi.softAP(_accessPointCredentials.ssid, _accessPointCredentials.password)) {
        _accessPointState = WifiAccessPointState::OFF;
        return false;
    }
    _mode = WifiMode::AP;
    _accessPointState = WifiAccessPointState::ACTIVE;
    return true;
}

void WifiController::_stopStation() {
    WiFi.disconnect(true);
    _stationState = WifiStationState::OFF;
}

void WifiController::_stopAccessPoint() {
    WiFi.softAPdisconnect(true);
    _accessPointState = WifiAccessPointState::OFF;
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
    if ((_mode != WifiMode::STA && _mode != WifiMode::APSTA)
            || _stationState != WifiStationState::DISCONNECTED
            || _cfg.reconnectIntervalMs == 0
            || nowMs - _lastReconnectMs < _cfg.reconnectIntervalMs)
        return;
    _lastReconnectMs = nowMs;
    ++_reconnectAttempts;
    _startStation();
}

void WifiController::setConfig(const WifiConfig &cfg) {
    _cfg = cfg;
    _applyConfiguration();
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
    _stationState = WifiStationState::OFF;
    _accessPointState = WifiAccessPointState::OFF;
    _lastReconnectMs = 0;
    _reconnectAttempts = 0;
}

bool WifiController::isConnected() const { return _stationState == WifiStationState::CONNECTED; }

bool WifiController::isAccessPointActive() const { return _accessPointState == WifiAccessPointState::ACTIVE; }

const char* WifiController::getHostname() const { return _cfg.hostname; }

WifiSnapshot WifiController::getSnapshot() const {
    WifiSnapshot snapshot{};
    snapshot.mode = _mode;
    strlcpy(snapshot.hostname, _cfg.hostname, sizeof(snapshot.hostname));
    snapshot.station.state = _stationState;
    snapshot.station.reconnectAttempts = _reconnectAttempts;
    if (isConnected()) {
        snapshot.station.ip = WiFi.localIP();
        snapshot.station.rssi = WiFi.RSSI();
        strlcpy(
            snapshot.station.ssid,
            _stationCredentials.ssid,
            sizeof(snapshot.station.ssid)
        );
    }
    snapshot.accessPoint.state = _accessPointState;
    if (isAccessPointActive()) {
        snapshot.accessPoint.ip = WiFi.softAPIP();
        snapshot.accessPoint.clients = WiFi.softAPgetStationNum();
        strlcpy(
            snapshot.accessPoint.ssid,
            _accessPointCredentials.ssid,
            sizeof(snapshot.accessPoint.ssid)
        );
    }
    return snapshot;
}
