#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H
#include <Arduino.h>
#include "nvs_flash.h"
#include <Timer.h>
#include "storage/SystemStorage.h"

#include <ESPAsyncWebServer.h>

struct HeapStatus {
    uint32_t free;
    uint32_t total;
    uint32_t maxAlloc;
};

struct SystemStatus {
    uint32_t uptime;
    HeapStatus heap;
};

class SystemController {
private:
    SystemStorage &_storage;
    Timer &_statusReportTimer;
public:
    SystemController(SystemStorage &storage, Timer &statusReportTimer);
    void init();
    bool tick();
    void restart();
    void factoryReset();
    void resetConfig();
    void getHeapStatus(HeapStatus &target) const;
    void getStatus(SystemStatus &target) const;
    void getConfig(SystemConfig &target) const;
    void updateConfig(const SystemConfig &cfg);
};
#endif
