#include "WifiController.h"

WifiController::WifiController(Timer *timer) : _timer(timer) {}

bool WifiController::init(IPAddress localIP, IPAddress gateway, IPAddress subnet, 
                        IPAddress primaryDNS, IPAddress secondaryDNS, uint32_t msInterval) {
    WiFi.mode(WIFI_STA);
    _timer->setTimeout(msInterval);
    return WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
}

bool WifiController::tick() {
    if (!_timer->tick())
        return false;
    bool res = checkConnectivity();
    _timer->refresh();
    return res;
}

wl_status_t WifiController::connect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
    _timer->start();
    return WiFi.status();
}

bool WifiController::checkConnectivity() {
    return isConnected() || WiFi.reconnect();
}


bool WifiController::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiController::getIP() { return WiFi.localIP(); }

int8_t WifiController::getRSSI() { return WiFi.RSSI(); }
