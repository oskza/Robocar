#ifndef ROBOCAR_H
#define ROBOCAR_H
#include <CompassBMM150.h>
#include "MotorController/MotorController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include "WebSocketController/WebSocketController.h"
#include "WifiController/WifiController.h"
#include <net_defaults.h>
#include <wifi_creds.h>

class Robocar {
private:
    MotorController &_motorController;
    WifiController &_wifiController;
    WebSocketController &_wsController;
    AnalogJoysticController &_joysticController;
    CompassBMM150 &_compass;
    Timer &_timer;
    AsyncWebServer &_server;
    static Robocar *_instance;
    static void _handleCommandStatic(JsonDocument &doc);
public:
    Robocar(MotorController &motorController, WifiController &wifiController, 
                WebSocketController &wsController, AnalogJoysticController &joysticController, 
                CompassBMM150 &compass, Timer &timer, AsyncWebServer &server);
    void init(uint32_t freq, uint8_t res, uint32_t statusIntervalMs, 
                uint32_t wifiIntervalMs, uint32_t wsIntervalMs, uint32_t joysticIntervalMs);
    void tick();
    void handleCommand(JsonDocument &doc);
    void handleDriveAutoCommand(JsonObject &payload);
    void handleDriveManualCommand(JsonObject &payload);
    void createStatus(JsonDocument &doc);
    void getHeapMetrics(JsonObject &target);
};
#endif
