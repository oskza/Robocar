#ifndef ROBOCAR_H
#define ROBOCAR_H
#include "DriveController/DriveController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include "WebSocketController/WebSocketController.h"
#include "WifiController/WifiController.h"
#include <net_defaults.h>
#include <wifi_creds.h>

class Robocar {
private:
    DriveController &_driveController;
    WifiController &_wifiController;
    WebSocketController &_wsController;
    AnalogJoysticController &_joysticController;
    Timer &_timer;
    AsyncWebServer &_server;
    static Robocar *_instance;
    static void _handleCommandStatic(JsonDocument &doc);
public:
    Robocar(DriveController &driveController, WifiController &wifiController, 
                WebSocketController &wsController, AnalogJoysticController &joysticController, 
                Timer &timer, AsyncWebServer &server);
    void init(uint32_t freq, uint8_t res, uint32_t statusIntervalMs, 
                uint32_t wifiIntervalMs, uint32_t wsIntervalMs, uint32_t joysticIntervalMs);
    void tick();
    void handleCommand(JsonDocument &doc);
    void handleAutoDrive(JsonObject &payload);
    void handleManualDrive(JsonObject &payload);
    void createStatus(JsonDocument &doc);
    void getHeapMetrics(JsonObject &target);
};
#endif
