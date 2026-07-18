#include "WifiController.h"

#include <cstring>

WifiController *WifiController::_instance = nullptr;

WifiController::WifiController()
    : _cfg{},
      _stationCredentials{},
      _accessPointCredentials{},
      _mode(WifiMode::OFF),
      _state{},
      _stationConnectTimer{},
      _reconnectTimer{},
      _reconnectAttempts(0),
      _eventRegistered(false) {}

void WifiController::_onWifiEvent(WiFiEvent_t event) {
    if (_instance != nullptr) _instance->_handleWifiEvent(event);
}

void WifiController::_handleWifiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_START:
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            _reconnectTimer.stop();
            _state.station = _stationEnabled()
                ? WifiStationState::CONNECTING
                : WifiStationState::OFF;
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            if (!_stationEnabled())
                break;
            _state.station = WifiStationState::CONNECTED;
            _reconnectAttempts = 0;
            _stopStationTimers();
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            _stationConnectTimer.stop();
            _reconnectTimer.stop();
            _state.station = (_stationEnabled())
                ? WifiStationState::DISCONNECTED
                : WifiStationState::OFF;
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            _state.accessPoint = (_accessPointEnabled())
                ? WifiAccessPointState::ACTIVE
                : WifiAccessPointState::OFF;
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            _state.accessPoint = WifiAccessPointState::OFF;
            break;
        default:
            break;
    }
}

bool WifiController::_registerEventHandler() {
    if (_instance != nullptr && _instance != this)
        return false;
    _instance = this;
    if (!_eventRegistered) {
        WiFi.onEvent(_onWifiEvent);
        _eventRegistered = true;
    }
    return true;
}

bool WifiController::_hasStationCredentials() const { return _stationCredentials.ssid[0] != '\0'; }

bool WifiController::_hasAccessPointCredentials() const { return _accessPointCredentials.ssid[0] != '\0'; }

bool WifiController::_stationConfigured() const { return _cfg.mode == WifiMode::STA || _cfg.mode == WifiMode::APSTA; }

bool WifiController::_accessPointConfigured() const { return _cfg.mode == WifiMode::AP || _cfg.mode == WifiMode::APSTA; }

bool WifiController::_stationEnabled() const { return _mode == WifiMode::STA || _mode == WifiMode::APSTA; }

bool WifiController::_accessPointEnabled() const { return _mode == WifiMode::AP || _mode == WifiMode::APSTA; }

bool WifiController::_usingAccessPointFallback() const {
    return _mode == WifiMode::AP && _cfg.accessPointFallback && _stationConfigured();
}

void WifiController::_stopStationTimers() {
    _stationConnectTimer.stop();
    _reconnectTimer.stop();
}

void WifiController::_applyHostname() {
    if (_cfg.hostname[0] != '\0')
        WiFi.setHostname(_cfg.hostname);
}

bool WifiController::_applyIpConfig() {
    return (_cfg.dhcp)
        ? WiFi.config(
            INADDR_NONE,
            INADDR_NONE,
            INADDR_NONE
        )
        : WiFi.config(
            _cfg.staticIp.ip,
            _cfg.staticIp.gateway,
            _cfg.staticIp.subnet,
            _cfg.staticIp.primaryDns,
            _cfg.staticIp.secondaryDns
        );
}

bool WifiController::_startStation() {
    _stationConnectTimer.stop();
    _reconnectTimer.stop();
    if (!_hasStationCredentials()) {
        _state.station = WifiStationState::OFF;
        return false;
    }
    _applyHostname();
    if (!_applyIpConfig()) {
        _state.station = WifiStationState::DISCONNECTED;
        return false;
    }
    if (WiFi.begin(_stationCredentials.ssid, _stationCredentials.password) == WL_CONNECT_FAILED) {
        _state.station = WifiStationState::DISCONNECTED;
        return false;
    }
    _state.station = WifiStationState::CONNECTING;
    return true;
}

bool WifiController::_startAccessPoint() {
    if (!_hasAccessPointCredentials()) {
        _state.accessPoint = WifiAccessPointState::OFF;
        return false;
    }
    if (!WiFi.softAP(_accessPointCredentials.ssid, _accessPointCredentials.password)) {
        _state.accessPoint = WifiAccessPointState::OFF;
        return false;
    }
    _state.accessPoint = WifiAccessPointState::ACTIVE;
    return true;
}

void WifiController::_stopStation() {
    WiFi.disconnect(false, false);
    _state.station = WifiStationState::OFF;
    _stopStationTimers();
}

void WifiController::_stopAccessPoint() {
    WiFi.softAPdisconnect(false);
    _state.accessPoint = WifiAccessPointState::OFF;
}

bool WifiController::_applyMode(WifiMode mode) {
    switch (mode) {
        case WifiMode::OFF:
            disconnect();
            return true;
        case WifiMode::STA:
            _mode = WifiMode::STA;
            _stopAccessPoint();
            if (!WiFi.mode(WIFI_STA)) {
                _mode = WifiMode::OFF;
                _state.station = WifiStationState::OFF;
                return false;
            }
            return _startStation();
        case WifiMode::AP:
            _mode = WifiMode::AP;
            _stopStation();
            if (!WiFi.mode(WIFI_AP)) {
                _mode = WifiMode::OFF;
                _state.accessPoint = WifiAccessPointState::OFF;
                return false;
            }
            return _startAccessPoint();
        case WifiMode::APSTA: {
            _mode = WifiMode::APSTA;
            _stopStation();
            _stopAccessPoint();
            if (!WiFi.mode(WIFI_AP_STA)) {
                _mode = WifiMode::OFF;
                return false;
            }
            const bool accessPointStarted = _startAccessPoint();
            const bool stationStarted = _startStation();
            return accessPointStarted && stationStarted;
        }
    }
    return false;
}

bool WifiController::_applyConfiguration() {
    if (_applyMode(_cfg.mode))
        return true;
    if (!_cfg.accessPointFallback ||
        !_stationConfigured()) {
        disconnect();
        return false;
    }
    return _applyMode(WifiMode::AP);
}

void WifiController::_updateConnecting(uint32_t nowMs) {
    if (_state.station != WifiStationState::CONNECTING ||
        _cfg.stationConnectTimeoutMs == 0) {
        return;
    }
    if (!_stationConnectTimer.active) {
        _stationConnectTimer.start(nowMs);
        return;
    }
    if (!_stationConnectTimer.elapsed(nowMs, _cfg.stationConnectTimeoutMs))
        return;
    WiFi.disconnect(false, false);
    _state.station = WifiStationState::DISCONNECTED;
    _stationConnectTimer.stop();
    if (_cfg.reconnectIntervalMs == 0) {
        if (_cfg.accessPointFallback && _stationConfigured())
            _applyMode(WifiMode::AP);
        return;
    }
    _reconnectTimer.start(nowMs);
}

void WifiController::_updateReconnect(uint32_t nowMs) {
    if (_state.station != WifiStationState::DISCONNECTED || _cfg.reconnectIntervalMs == 0)
        return;
    if (!_reconnectTimer.active) {
        _reconnectTimer.start(nowMs);
        return;
    }
    if (!_reconnectTimer.elapsed(nowMs, _cfg.reconnectIntervalMs))
        return;
    if (_cfg.maxReconnectAttempts > 0 && _reconnectAttempts >= _cfg.maxReconnectAttempts) {
        if (_cfg.accessPointFallback && _stationConfigured())
            _applyMode(WifiMode::AP);
        return;
    }
    ++_reconnectAttempts;
    if (_startStation())
        return;
    _state.station = WifiStationState::DISCONNECTED;
    _reconnectTimer.start(nowMs);
}

bool WifiController::begin(
    const WifiConfig &config,
    const WifiCredentials &stationCredentials,
    const WifiCredentials &accessPointCredentials
) {
    if (!_registerEventHandler())
        return false;
    _cfg = config;
    _stationCredentials = stationCredentials;
    _accessPointCredentials = accessPointCredentials;
    _mode = WifiMode::OFF;
    _state.station = WifiStationState::OFF;
    _state.accessPoint = WifiAccessPointState::OFF;
    _reconnectAttempts = 0;
    _stopStationTimers();
    return _applyConfiguration();
}

void WifiController::update(uint32_t nowMs) {
    if (!_stationEnabled())
        return;
    switch (_state.station) {
        case WifiStationState::CONNECTING:
            _updateConnecting(nowMs);
            break;
        case WifiStationState::DISCONNECTED:
            _updateReconnect(nowMs);
            break;
        case WifiStationState::OFF:
        case WifiStationState::CONNECTED:
            break;
    }
}

void WifiController::disconnect() {
    _mode = WifiMode::OFF;
    _stopStation();
    _stopAccessPoint();
    WiFi.mode(WIFI_OFF);
    _state.station = WifiStationState::OFF;
    _state.accessPoint = WifiAccessPointState::OFF;
    _reconnectAttempts = 0;
    _stopStationTimers();
}

bool WifiController::isConnected() const { return _state.station == WifiStationState::CONNECTED; }

bool WifiController::isAccessPointActive() const { return _state.accessPoint == WifiAccessPointState::ACTIVE; }

const char *WifiController::getHostname() const { return _cfg.hostname; }

WifiSnapshot WifiController::getSnapshot() const {
    WifiSnapshot snapshot{};
    snapshot.mode = _mode;
    strlcpy(snapshot.hostname, _cfg.hostname, sizeof(snapshot.hostname));
    snapshot.station.state = _state.station;
    snapshot.station.reconnectAttempts = _reconnectAttempts;
    if (isConnected()) {
        snapshot.station.ip = WiFi.localIP();
        snapshot.station.rssi = WiFi.RSSI();
        strlcpy(snapshot.station.ssid, _stationCredentials.ssid, sizeof(snapshot.station.ssid));
    }
    snapshot.accessPoint.state = _state.accessPoint;
    if (isAccessPointActive()) {
        snapshot.accessPoint.ip = WiFi.softAPIP();
        snapshot.accessPoint.clients = WiFi.softAPgetStationNum();
        strlcpy(snapshot.accessPoint.ssid, _accessPointCredentials.ssid, sizeof(snapshot.accessPoint.ssid));
    }
    return snapshot;
}

void WifiController::getConfig(WifiConfig &config) const { config = _cfg; }

bool WifiController::setConfig(const WifiConfig &config) {
    _cfg = config;
    _reconnectAttempts = 0;
    _stopStationTimers();
    return _applyConfiguration();
}

bool WifiController::setStationCredentials(const WifiCredentials &credentials) {
    _stationCredentials = credentials;
    _reconnectAttempts = 0;
    _stopStationTimers();
    return _stationConfigured() ? _applyConfiguration() : true;
}

void WifiController::getAccessPointCredentials(WifiCredentials &credentials) const { credentials = _accessPointCredentials; }

bool WifiController::setAccessPointCredentials(const WifiCredentials &credentials) {
    _accessPointCredentials = credentials;
    return (_accessPointConfigured() || _usingAccessPointFallback()) ? _applyConfiguration() : true;
}
