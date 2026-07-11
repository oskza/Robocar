#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H
#include <WiFi.h>
#include "WifiConfig.h"
#include "WifiCredentials.h"
#include "WifiSnapshot.h"

class WifiController {
private:
    WifiConfig _cfg;
    WifiCredentials _stationCredentials;
    WifiCredentials _accessPointCredentials;
    WifiMode _mode;
    WifiStationState _stationState;
    WifiAccessPointState _accessPointState;
    uint32_t _lastReconnectMs;
    uint32_t _reconnectAttempts;
    static WifiController *_instance;
    static void _onWifiEvent(WiFiEvent_t event);
    void _handleWifiEvent(WiFiEvent_t event);
    bool _hasStationCredentials() const;
    bool _hasAccessPointCredentials() const;
    bool _applyMode(WifiMode mode);
    void _applyConfiguration();
    void _applyHostname();
    void _applyIpConfig();
    bool _startStation();
    bool _startAccessPoint();
    void _stopStation();
    void _stopAccessPoint();
public:
    WifiController();
    void begin(
        const WifiConfig &cfg,
        const WifiCredentials &stationCredentials,
        const WifiCredentials &accessPointCredentials
    );
    void update(uint32_t nowMs);
    void disconnect();
    bool isConnected() const;
    bool isAccessPointActive() const;
    const char *getHostname() const;
    WifiSnapshot getSnapshot() const;
    void getConfig(WifiConfig &cfg) const;
    void setConfig(const WifiConfig &cfg);
    void setStationCredentials(const WifiCredentials &credentials);
    void getAccessPointCredentials(WifiCredentials &credentials) const;
    void setAccessPointCredentials(const WifiCredentials &credentials);
};
#endif
