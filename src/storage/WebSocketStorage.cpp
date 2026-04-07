#include "WebSocketStorage.h"

#define PREFS_WS_CFG            "ws_cfg"
#define PREFS_MAX_CLIENTS_KEY   "max_clients"
#define PREFS_INTERVAL_KEY      "interval"

void WebSocketStorage::begin() { _prefs.begin(PREFS_WS_CFG, false); }

void WebSocketStorage::reset() { _prefs.clear(); }

void WebSocketStorage::loadConfig(WebSocketConfig &target) {
    target.maxClients = loadMaxClients();
    target.intervalMs = loadIntervalMs();
}

uint8_t WebSocketStorage::loadMaxClients() { return _prefs.getUChar(PREFS_MAX_CLIENTS_KEY, WebSocketDefaults::maxClients); }

uint32_t WebSocketStorage::loadIntervalMs() { return _prefs.getULong(PREFS_INTERVAL_KEY, WebSocketDefaults::intervalMs); }

void WebSocketStorage::saveConfig(WebSocketConfig &cfg) {
    saveMaxClients(cfg.maxClients);
    saveIntervalMs(cfg.intervalMs);
}

void WebSocketStorage::saveMaxClients(uint8_t clients) { _prefs.putUChar(PREFS_MAX_CLIENTS_KEY, clients); }

void WebSocketStorage::saveIntervalMs(uint32_t ms) { _prefs.putULong(PREFS_INTERVAL_KEY, ms); }
