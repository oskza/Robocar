#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H
#include "WiFi.h"
#include <Timer.h>
#include "storage/NetworkStorage.h"

struct WifiAPStatus {
    uint8_t clients;
};

struct WifiSTAStatus {
    bool connected;
    int8_t rssi;
};

struct NetworkStatus {
    char mode[6];
    union {
        WifiAPStatus ap;
        WifiSTAStatus sta;
    };
};

class NetworkController {
private:
    NetworkStorage &_storage;
    Timer &_timer;
    bool _configureStation();
    void _setDHCP(bool enable);
public:
    NetworkController(NetworkStorage &storage, Timer &timer);
    static void modeToString(uint8_t mode, char *target);
    bool init();
    bool tick();
    wl_status_t connect();
    void disconnect();
    bool checkConnectivity();
    bool enableAP();
    void enableDHCP();
    void disableDHCP();
    bool isConnected() const;
    bool hasCredentials() const;
    void reset();
    void resetConfig();
    void resetCredentials();
    void resetAPCredentials();
    void getStatus(NetworkStatus &target) const;
    void getConfig(NetworkConfig &target) const;
    void getCredentials(WifiCredentials &target) const;
    void getAPCredentials(WifiCredentials &target) const;
    void updateConfig(const NetworkConfig &cfg);
    void updateCredentials(const WifiCredentials &creds);
    void updateAPCredentials(const WifiCredentials &creds);
};
#endif
