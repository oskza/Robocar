#include "WifiManager.h"

WifiManager::WifiManager(Timer *timer) : _timer(timer) {}

bool WifiManager::init(IPAddress localIP, IPAddress gateway, IPAddress subnet, 
                        IPAddress primaryDNS, IPAddress secondaryDNS,
                        uint32_t msCheckInterval) {
    WiFi.mode(WIFI_STA);
    _timer->setTimeout(msCheckInterval);
    return WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
}

wl_status_t WifiManager::connect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
    _timer->start();
    return WiFi.status();
}

bool WifiManager::checkConnectivity() {
    return WiFi.status() == WL_CONNECTED || WiFi.reconnect();
}

bool WifiManager::tick() {
    if (!_timer->tick())
        return false;
    bool res = checkConnectivity();
    _timer->refresh();
    return res;
}

IPAddress WifiManager::getIP() { return WiFi.localIP(); }
