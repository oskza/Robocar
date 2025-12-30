#include "Robocar.h"

Robocar *Robocar::_instance = nullptr;

Robocar::Robocar(MotorController &motorController, WifiController &wifiController, 
                    WebSocketController &wsController, AnalogJoysticController &joysticController, 
                    CompassBMM150 &compass, Timer &timer, AsyncWebServer &server)
                : _motorController(motorController), _wifiController(wifiController),
                    _wsController(wsController), _joysticController(joysticController),
                    _compass(compass), _timer(timer), _server(server) { _instance = this; }

void Robocar::_handleCommandStatic(JsonDocument &doc) {
    if (_instance)
        _instance->handleCommand(doc);
}

void Robocar::init(uint32_t freq, uint8_t res, uint32_t statusIntervalMs, 
                    uint32_t wifiIntervalMs, uint32_t wsIntervalMs, uint32_t joysticIntervalMs) {
    if (!_compass.init()) {/*...*/}

    _motorController.init(freq, res);

    _joysticController.init(joysticIntervalMs);

    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!_wifiController.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, wifiIntervalMs)
            || _wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {/*...*/}

    _wsController.init(_server, _handleCommandStatic, wsIntervalMs);
    _server.begin();

    _timer.setTimeout(statusIntervalMs);
    _timer.start();
}

void Robocar::tick() {
    int16_t vert, horz;
    if(_joysticController.tick(vert, horz)
            && (vert != 0 || horz != 0 || _motorController.isDriving()))
        _motorController.driveDifferential(vert, horz);
    else
        _motorController.tick();

    if (_wifiController.tick()) {/*...*/}

    _wsController.tick();

    if (_timer.tick()) {
        if (_wsController.hasClients()) {
            StaticJsonDocument<512> status;
            createStatus(status);
            _wsController.sendAll(status);
        }
        _timer.refresh();
    }
}

void Robocar::handleCommand(JsonDocument &doc) {
    const char *cmd = doc["cmd"] | "";
    if (!cmd) 
        return;
    if (strcmp(cmd, "set-mode") == 0) {
        const char *mode = doc["payload"] | "";
        _motorController.setMode(mode);
        return;
    }
    if (strcmp(cmd, "stop") == 0) {
        _motorController.stop();
        return;
    }
    if (strcmp(cmd, "drive") == 0) {
        JsonObject payload = doc["payload"];
        if (!payload) 
            return;
        if(_motorController.isModeManual()) {
            handleDriveManualCommand(payload);
            return;
        }
        handleDriveAutoCommand(payload);
    }
}

void Robocar::handleDriveAutoCommand(JsonObject &payload) {
    int16_t velocity = payload["velocity"] | 0;
    const char *navigation = payload["navigation"] | "";
    if (!navigation)
        return;
    if (strcmp(navigation, "duration") == 0) {
        int16_t turn = payload["turn"] | 0;
        uint32_t ms = payload["duration"] | 0;
        _motorController.driveFor(velocity, turn, ms);
        return;
    }
    if (strcmp(navigation, "distance") == 0) {
        float meters = payload["distance"] | 0.00f;
        _motorController.driveDistance(velocity, meters);
        return;
    }
}

void Robocar::handleDriveManualCommand(JsonObject &payload) {
    int16_t velocity = payload["velocity"] | 0;
    int16_t turn = payload["turn"] | 0;
    const char *control = payload["control"] | "";
    if (!control)
        return;
    if (strcmp(control, "joystic") == 0) {
        _motorController.driveDifferential(velocity, turn);
        return;
    }
    if (strcmp(control, "keyboard") == 0) {
        bool up = payload["up"] | false;
        bool down = payload["down"] | false;
        bool right = payload["right"] | false;
        bool left = payload["left"] | false;
        _motorController.driveDiscreteArcade(velocity, turn, up, down, right, left);
    }
}

void Robocar::createStatus(JsonDocument &doc) {
    doc["type"] = "status";
    doc["uptime"] = millis();
    doc["clients"] = _wsController.getClientsCount();
    doc["heading"] = _compass.getCompassDegree();
    JsonObject heap = doc["heap"].to<JsonObject>();
    getHeapMetrics(heap);
    JsonObject drive = doc["drive"].to<JsonObject>();
    _motorController.getStatus(drive);
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    _wifiController.getStatus(wifi);
}

void Robocar::getHeapMetrics(JsonObject &target) {
    target["free"] = ESP.getFreeHeap();
    target["total"] = ESP.getHeapSize();
    target["maxAlloc"] = ESP.getMaxAllocHeap();
}
