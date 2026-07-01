#include "WifiManager.h"

bool WifiManager::begin(const char *ssid, const char *password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    _connected = false;
    return true;
}

void WifiManager::update() { _connected = (WiFi.status() == WL_CONNECTED); }

bool WifiManager::isConnected() const { return _connected; }

IPAddress WifiManager::getLocalIp() const { return WiFi.localIP(); }

int32_t WifiManager::getRssi() const { return (_connected) ? WiFi.RSSI() : 0; }
