#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H
#include <WiFi.h>
#include "WifiState.h"
#include "WifiConfig.h"
#include "WifiCredentials.h"
#include "WifiSnapshot.h"

class WifiController {
private:
    struct TimerState {
        uint32_t startedMs = 0;
        bool active = false;
        void start(uint32_t nowMs) {
            startedMs = nowMs;
            active = true;
        }
        void stop() {
            startedMs = 0;
            active = false;
        }
        bool elapsed(uint32_t nowMs, uint32_t durationMs) const {
            return active && (nowMs - startedMs) >= durationMs;
        }
    };
    WifiConfig _cfg;
    WifiCredentials _stationCredentials;
    WifiCredentials _accessPointCredentials;
    WifiMode _mode;
    WifiState _state;
    TimerState _stationConnectTimer;
    TimerState _reconnectTimer;
    uint32_t _reconnectAttempts;
    bool _eventRegistered;
    static WifiController *_instance;
    static void _onWifiEvent(WiFiEvent_t event);
    void _handleWifiEvent(WiFiEvent_t event);
    bool _registerEventHandler();
    bool _hasStationCredentials() const;
    bool _hasAccessPointCredentials() const;
    bool _stationConfigured() const;
    bool _accessPointConfigured() const;
    bool _stationEnabled() const;
    bool _accessPointEnabled() const;
    bool _usingAccessPointFallback() const;
    void _stopStationTimers();
    bool _applyConfiguration();
    bool _applyMode(WifiMode mode);
    void _applyHostname();
    bool _applyIpConfig();
    bool _startStation();
    bool _startAccessPoint();
    void _stopStation();
    void _stopAccessPoint();
    void _updateConnecting(uint32_t nowMs);
    void _updateReconnect(uint32_t nowMs);
public:
    WifiController();
    bool begin(
        const WifiConfig &config,
        const WifiCredentials &stationCredentials,
        const WifiCredentials &accessPointCredentials
    );
    void update(uint32_t nowMs);
    void disconnect();
    bool isConnected() const;
    bool isAccessPointActive() const;
    const char *getHostname() const;
    WifiSnapshot getSnapshot() const;
    void getConfig(WifiConfig &config) const;
    bool setConfig(const WifiConfig &config);
    bool setStationCredentials(const WifiCredentials &credentials);
    void getAccessPointCredentials(WifiCredentials &credentials) const;
    bool setAccessPointCredentials(const WifiCredentials &credentials);
};
#endif
