#include "Robocar.h"

Robocar *Robocar::_instance = nullptr;

Robocar::Robocar(DriveController &driveController, WifiController &wifiController, 
                    WebSocketController &wsController, AnalogJoysticController &joysticController, 
                    Timer &timer, AsyncWebServer &server)
                : _driveController(driveController), _wifiController(wifiController),
                    _wsController(wsController), _joysticController(joysticController),
                    _timer(timer), _server(server) { _instance = this; }

void Robocar::_handleCommandStatic(JsonDocument &doc) {
    if (_instance)
        _instance->handleCommand(doc);
}

void Robocar::init(uint32_t freq, uint8_t res, uint32_t statusIntervalMs, 
                    uint32_t wifiIntervalMs, uint32_t wsIntervalMs, uint32_t joysticIntervalMs) {
    _driveController.init(freq, res);

    // _joysticController.init(joysticIntervalMs);

    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!_wifiController.init(HOSTNAME, localIP, gateway, subnet, primaryDNS, secondaryDNS, wifiIntervalMs)
            || _wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {/*...*/}

    _wsController.init(_server, _handleCommandStatic, wsIntervalMs);
    _server.begin();

    _timer.setTimeout(statusIntervalMs);
    _timer.start();
}

void Robocar::tick() {
    int16_t vert, horz;
    if(_joysticController.tick(vert, horz)
            && (vert != 0 || horz != 0 || _driveController.isDriving()))
        _driveController.driveDifferential(vert, horz);
    else
        _driveController.tick();        

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
        _driveController.setMode(mode);
        return;
    }
    if (strcmp(cmd, "stop") == 0) {
        _driveController.stop();
        return;
    }
    if (strcmp(cmd, "drive") == 0) {
        JsonObject payload = doc["payload"];
        if (!payload) 
            return;
        if(_driveController.isModeManual()) {
            handleManualDrive(payload);
            return;
        }
        handleAutoDrive(payload);
    }
}

void Robocar::handleAutoDrive(JsonObject &payload) {
    const char *navigation = payload["navigation"] | "";
    if (!navigation)
        return;
    if (strcmp(navigation, "duration") == 0) {
        int16_t velocity = payload["velocity"] | 0;
        int16_t turn = payload["turn"] | 0;
        uint32_t ms = payload["duration"] | 0;
        _driveController.driveFor(velocity, turn, ms);
        return;
    }
    if (strcmp(navigation, "distance") == 0) {
        int16_t velocity = payload["velocity"] | 0;
        double meters = payload["distance"] | 0.00f;
        _driveController.driveDistance(velocity, meters);
        return;
    }
}

void Robocar::handleManualDrive(JsonObject &payload) {
    int16_t velocity = payload["velocity"] | 0;
    int16_t turn = payload["turn"] | 0;
    const char *control = payload["control"] | "";
    if (!control)
        return;
    if (strcmp(control, "joystic") == 0) {
        _driveController.driveDifferential(velocity, turn);
        return;
    }
    if (strcmp(control, "keyboard") == 0) {
        bool up = payload["up"] | false;
        bool down = payload["down"] | false;
        bool right = payload["right"] | false;
        bool left = payload["left"] | false;
        _driveController.driveDiscreteArcade(velocity, turn, up, down, right, left);
    }
}

void Robocar::createStatus(JsonDocument &doc)  const {
    doc["type"] = "status";
    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["uptime"] = millis();
    payload["clients"] = _wsController.getClientsCount();
    JsonObject heap = payload["heap"].to<JsonObject>();
    getHeapMetrics(heap);
    JsonObject drive = payload["drive"].to<JsonObject>();
    getDriveStatus(drive);
    JsonObject wifi = payload["wifi"].to<JsonObject>();
    getWifiStatus(wifi);
}

void Robocar::getHeapMetrics(JsonObject &target) const {
    target["free"] = ESP.getFreeHeap();
    target["total"] = ESP.getHeapSize();
    target["maxAlloc"] = ESP.getMaxAllocHeap();
}

/** TODO: add remaining meters/ms/degree */
void Robocar::getDriveStatus(JsonObject &target) const {
    bool driving = _driveController.isDriving();
    target["mode"] = _driveController.getMode();
    target["heading"] = _driveController.getHeading();
    target["driving"] = driving;
    if (driving) {
        target["pwmRight"] = _driveController.getRightPWM();
        target["pwmLeft"] = _driveController.getLeftPWM();
        target["distance"] = _driveController.getDistanceMeters();
        target["duration"] = _driveController.getDurationMs();
    }
}

void Robocar::getWifiStatus(JsonObject &target) const {
    bool connected = _wifiController.isConnected();
    target["connected"] = connected;
    if (connected) {
        target["ip"] = _wifiController.getIP();
        target["subnet"] = _wifiController.getSubnet();
        target["rssi"] = _wifiController.getRSSI();
    }
}
