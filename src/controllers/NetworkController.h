#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H
#include "WiFi.h"
#include <Timer.h>
#include "storage/NetworkStorage.h"

class NetworkController {
private:
    NetworkStorage &_storage;
    Timer &_timer;
    NetworkConfig _config;
public:
    NetworkController(NetworkStorage &storage, Timer &timer);
    bool init();
    bool tick();
    wl_status_t connect();
    bool checkConnectivity();
    bool isConnected() const;
    int8_t getRSSI() const;
    void getConfig(NetworkConfig &target) const;
    void updateConfig(NetworkConfig &cfg);
    void resetConfig();
    void updateHostname(const char *hostname);
    void updateSSID(const char *ssid);
    void updatePassword(const char *password);
    void updateLocalIP(const uint8_t *ip);
    void updateGateway(const uint8_t *gateway);
    void updateSubnet(const uint8_t *subnet);
    void updatePrimaryDNS(const uint8_t *dns);
    void updateSecondaryDNS(const uint8_t *dns);
    void updateIntervalMs(uint32_t ms);
};
#endif
