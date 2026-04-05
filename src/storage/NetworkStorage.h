#ifndef NETWORK_STORAGE_H
#define NETWORK_STORAGE_H
#include <Preferences.h>
#include "config/NetworkConfig.h"
#include "config/WifiCredentials.h"
#include "defaults/NetworkDefaults.h"

class NetworkStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void clear();
    void clearConfig();
    void clearCredentials();
    void clearAPCredentials();
    void loadConfig(NetworkConfig &target);
    void loadCredentials(WifiCredentials &target);
    void loadAPCredentials(WifiCredentials &target);
    bool loadDHCP();
    uint32_t loadIntervalMs();
    void loadHostname(char *target);
    void loadSSID(char *target);
    void loadPassword(char *target);
    void loadAPSSID(char *target);
    void loadAPPassword(char *target);
    void loadLocalIP(uint8_t *target);
    void loadGateway(uint8_t *target);
    void loadSubnet(uint8_t *target);
    void loadPrimaryDNS(uint8_t *target);
    void loadSecondaryDNS(uint8_t *target);
    void saveConfig(const NetworkConfig &cfg);
    void saveCredentials(const WifiCredentials &creds);
    void saveAPCredentials(const WifiCredentials &creds);
    void saveDHCP(bool dhcp);
    void saveIntervalMs(uint32_t ms);
    void saveHostname(const char *hostname);
    void saveLocalIP(const uint8_t *ip);
    void saveGateway(const uint8_t *gateway);
    void saveSubnet(const uint8_t *subnet);
    void savePrimaryDNS(const uint8_t *dns);
    void saveSecondaryDNS(const uint8_t *dns);
    void saveSSID(const char *ssid);
    void savePassword(const char *password);
    void saveAPSSID(const char *ssid);
    void saveAPPassword(const char *password);
};
#endif
