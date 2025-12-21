#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include "WiFi.h"
#include <Timer.h>

class WifiManager {
private:
    Timer *_timer;
public:
    WifiManager(Timer *timer);
    bool init(IPAddress localIP, IPAddress gateway, IPAddress subnet, 
                IPAddress primaryDNS, IPAddress secondaryDNS, 
                uint32_t msCheckInterval = 1000);
    wl_status_t connect(const char *ssid, const char *password);
    bool checkConnectivity();
    bool tick();
    IPAddress getIP();
};
#endif
