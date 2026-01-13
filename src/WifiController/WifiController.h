#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H
#include "WiFi.h"
#include <Timer.h>

class WifiController {
private:
    Timer *_timer;
public:
    WifiController(Timer *timer);
    bool init(const char *hostname, IPAddress localIP, IPAddress gateway, IPAddress subnet, 
                IPAddress primaryDNS, IPAddress secondaryDNS, uint32_t msInterval);
    bool tick();
    wl_status_t connect(const char *ssid, const char *password);
    bool checkConnectivity();
    bool isConnected() const;
    IPAddress getIP() const;
    int8_t getRSSI() const;
    IPAddress getSubnet() const;
};
#endif
