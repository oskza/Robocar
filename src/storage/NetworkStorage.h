#ifndef NETWORK_STORAGE_H
#define NETWORK_STORAGE_H
#include <Preferences.h>
#include "config/NetworkConfig.h"
#include "defaults/NetworkDefaults.h"

class NetworkStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(NetworkConfig &target);
    void saveConfig(NetworkConfig &cfg);
    void loadHostname(char *target, size_t len);
    void saveHostname(const char *hostname);
    void loadSSID(char *target, size_t len);
    void saveSSID(const char *ssid);
    void loadPassword(char *target, size_t len);
    void savePassword(const char *password);
    void loadLocalIP(uint8_t *target);
    void saveLocalIP(const uint8_t *ip);
    void loadGateway(uint8_t *target);
    void saveGateway(const uint8_t *gateway);
    void loadSubnet(uint8_t *target);
    void saveSubnet(const uint8_t *subnet);
    void loadPrimaryDNS(uint8_t *target);
    void savePrimaryDNS(const uint8_t *dns);
    void loadSecondaryDNS(uint8_t *target);
    void saveSecondaryDNS(const uint8_t *dns);
    uint32_t loadIntervalMs();
    void saveIntervalMs(uint32_t ms);
};
#endif
