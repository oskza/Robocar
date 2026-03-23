#ifndef WEBSOCKET_STORAGE_H
#define WEBSOCKET_STORAGE_H
#include <Preferences.h>
#include "config/WebSocketConfig.h"
#include "defaults/WebSocketDefaults.h"

class WebSocketStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(WebSocketConfig &target);
    void saveConfig(WebSocketConfig &cfg);
    uint8_t loadMaxClients();
    void saveMaxClients(uint8_t clients);
    uint32_t loadIntervalMs();
    void saveIntervalMs(uint32_t ms);
};
#endif
