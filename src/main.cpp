#include <Arduino.h>
#include <CompassBMM150.h>
#include "MotionController/MotionController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include <WifiController/WifiController.h>
#include <wifi_creds.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define MOTOR_R_PWM_PIN         25
#define MOTOR_R_FWD_PIN         18
#define MOTOR_R_BCK_PIN         19

#define MOTOR_L_PWM_PIN         26
#define MOTOR_L_FWD_PIN         27
#define MOTOR_L_BCK_PIN         14

#define MOTOR_R_PWM_CHANNEL     0
#define MOTOR_L_PWM_CHANNEL     1

#define MOTOR_PWM_FREQ          1000
#define MOTOR_PWM_RES           8

#define MOTOR_L_CORR            1.0395f

#define ENCODER_R_PIN           32
#define ENCODER_L_PIN           33

#define JOYSTIC_VERT_PIN        34
#define JOYSTIC_HORZ_PIN        35
#define JOYSTIC_DEADZONE        7
#define JOYSTIC_INTERVAL_MS     20

#define WIFI_INTERVAL_MS        3000

#define WEBSOCKET_PORT          80
#define WEBSOCKET_MAX_CLIENTS   2
#define WEBSOCKET_INTERVAL_MS   2000

#define REPORT_INTERVAL_MS      5000

Timer timerMotion, timerJoystic, timerWifi, timerWebSocket, timerReport;

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);
MotionController motionController(&motorR, &motorL, &encoderR, &encoderL, &timerMotion);

CompassBMM150 compass;

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(&joystic, &timerJoystic, JOYSTIC_DEADZONE);

WifiController wifiController(&timerWifi);

static AsyncWebServer server(WEBSOCKET_PORT);
static AsyncWebSocket ws("/ws");

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

void handleCommand(JsonDocument &doc) {
    const char* cmd = doc["cmd"];
    if (!cmd) 
        return;
    if (strcmp(cmd, "set-mode") == 0) {
        motionController.setMode(doc["payload"]);
        return;
    }
    if (strcmp(cmd, "stop") == 0) {
        motionController.stop();
        return;
    }
    if (strcmp(cmd, "move") == 0) {

        if(motionController.isModeManual()) {
            const char *dir = doc["payload"];

            if (strcmp(dir, "forward") == 0) {
                motionController.moveForward();
                return;
            }
            if (strcmp(dir, "backward") == 0) {
                motionController.moveBackward();
                return;
            }
            if (strcmp(dir, "left") == 0) {
                motionController.moveLeft();
                return;
            }
            if (strcmp(dir, "right") == 0) {
                motionController.moveRight();
            }
            return;
        }

        int16_t velocity = doc["payload"]["velocity"] | 0;
        int16_t turn = doc["payload"]["turn"] | 0;
        JsonObject dur = doc["payload"]["duration"];
        const char *durUnit = dur["unit"] | "ms";
        float durValue = dur["value"] | 0;

        if (durValue == 0) {
            motionController.moveManual(velocity, turn);
            return;
        }

        if (strcmp(durUnit, "ms") == 0) {
            motionController.moveForMs(velocity, turn, durValue);
            return;
        }

        if (strcmp(durUnit, "meters") == 0) {
            motionController.moveForMeters(velocity, turn, durValue);
        }
    }
}

void wsSendJson(JsonDocument &doc) {
    char buffer[512];
    serializeJson(doc, buffer, sizeof(buffer));
    ws.textAll(buffer);
}

void wsHandleText(AsyncWebSocketClient *client, char *data) {
    StaticJsonDocument<256> doc;

    DeserializationError err = deserializeJson(doc, data);
    if (err) {
        client->text("{\"type\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }

    const char* type = doc["type"];
    if (!type) 
        return;

    if (strcmp(type, "command") == 0) {
        handleCommand(doc);
    }
}

void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    (void)len;
    switch (type) {
        case WS_EVT_CONNECT:
            client->setCloseClientOnQueueFull(false);
            client->ping();
            break;
        case WS_EVT_DISCONNECT:
            //...
            break;
        case WS_EVT_ERROR:
            //...
            break;
        case WS_EVT_PONG:
            //...
            break;
        case WS_EVT_DATA:
            AwsFrameInfo *info = (AwsFrameInfo *)arg;
            if (info->final && info->opcode == WS_TEXT) {
                data[len] = 0;
                wsHandleText(client, (char*)data);
            }
            break;
    }
}

void wsMiddleware(AsyncWebServerRequest *request, ArMiddlewareNext next) {
    if (ws.count() >= WEBSOCKET_MAX_CLIENTS)
        request->send(503, "text/plain", "Server is busy");
    else
        next();
}

void wsInit() {
    ws.onEvent(wsOnEvent);
    server.addHandler(&ws).addMiddleware(wsMiddleware);
    server.begin();
    timerWebSocket.setTimeout(WEBSOCKET_INTERVAL_MS);
    timerWebSocket.start();
}

void createReport(JsonDocument &doc) {
    doc["type"] = "report";
    doc["uptime"] = millis();

    JsonObject wifi = doc.createNestedObject("wifi");
    bool connected = wifiController.isConnected();
    wifi["connected"] = connected;
    if (connected)
        wifi["ip"] = wifiController.getIP();
        wifi["rssi"] = wifiController.getRSSI();

    doc["clients"] = ws.count();

    JsonObject memory = doc.createNestedObject("memory");
    memory["free"] = ESP.getFreeHeap();
    memory["total"] = ESP.getHeapSize();
    memory["maxAlloc"] = ESP.getMaxAllocHeap();

    doc["mode"] = (motionController.isModeAuto()) ? "auto" : "manual";
    doc["heading"] = compass.getCompassDegree();
}

void sendReport() {
    StaticJsonDocument<512> report;
    createReport(report);
    wsSendJson(report);
}

void setup() {
    Serial.begin(115200);

    motionController.init(onRightEncoder, onLeftEncoder, MOTOR_PWM_FREQ, MOTOR_PWM_RES);

    if (!compass.init()) {/*...*/}

    joysticController.init(JOYSTIC_INTERVAL_MS);

    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!wifiController.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, WIFI_INTERVAL_MS)) {/*...*/}
    if (wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {/*...*/}

    wsInit();

    timerReport.setTimeout(REPORT_INTERVAL_MS);
    timerReport.start();
}

void loop() {
    if (motionController.isModeManual()) {
        // int16_t vert, horz;
        // if(joysticController.tick(vert, horz)) {
        //     if(joysticController.isInDeadzone(vert, horz))
        //         motionController.stop();
        //     else
        //         motionController.moveManual(vert, horz);
        // }
    } else {
        motionController.tick();
    }
    
    if (wifiController.tick()) {/*...*/}

    if (timerWebSocket.tick()) {
        ws.cleanupClients(WEBSOCKET_MAX_CLIENTS);
        timerWebSocket.refresh();
    }

    if (timerReport.tick() && ws.count() > 0) {
        sendReport();
        timerReport.refresh();
    }
}
