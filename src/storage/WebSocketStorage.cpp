#include "WebSocketStorage.h"

void WebSocketStorage::begin() { _prefs.begin("ws_cfg", false); }

void WebSocketStorage::reset() { _prefs.clear(); }

void WebSocketStorage::loadConfig(WebSocketConfig &target) {
    target.maxClients = loadMaxClients();
    target.intervalMs = loadIntervalMs();
}

void WebSocketStorage::saveConfig(WebSocketConfig &cfg) {
    saveMaxClients(cfg.maxClients);
    saveIntervalMs(cfg.intervalMs);
}

uint8_t WebSocketStorage::loadMaxClients() { return _prefs.getUChar("max_clients", WebSocketDefaults::maxClients); }

void WebSocketStorage::saveMaxClients(uint8_t clients) { _prefs.putUChar("max_clients", clients); }

uint32_t WebSocketStorage::loadIntervalMs() { return _prefs.getULong("interval", WebSocketDefaults::intervalMs); }

void WebSocketStorage::saveIntervalMs(uint32_t ms) { _prefs.putULong("interval", ms); }
