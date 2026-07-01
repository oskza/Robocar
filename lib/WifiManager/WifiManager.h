#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include <WiFi.h>

class WifiManager {
private:
    bool _connected = false;
public:
    WifiManager() = default;
    bool begin(const char *ssid, const char *password);
    void update();
    bool isConnected() const;
    IPAddress getLocalIp() const;
};
#endif
