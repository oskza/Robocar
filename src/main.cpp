#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <CompassBMM150.h>
#include "MotionController/MotionController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include <WifiController/WifiController.h>
#include <wifi_creds.h>

#define MOTOR_R_PWM_PIN     25
#define MOTOR_R_FWD_PIN     18
#define MOTOR_R_BCK_PIN     19

#define MOTOR_L_PWM_PIN     26
#define MOTOR_L_FWD_PIN     27
#define MOTOR_L_BCK_PIN     14

#define MOTOR_R_PWM_CHANNEL 0
#define MOTOR_L_PWM_CHANNEL 1

#define MOTOR_PWM_FREQ     1000
#define MOTOR_PWM_RES      8

#define MOTOR_L_CORR        1.0395f

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

#define JOYSTIC_VERT_PIN    34
#define JOYSTIC_HORZ_PIN    35
#define JOYSTIC_DEADZONE    7
#define JOYSTIC_INTERVAL_MS 20

#define WIFI_CHECK_MS 3000

#define WEBSOCKET_PORT        80
#define WEBSOCKET_MAX_CLIENTS 2
#define WEBSOCKET_INTERVAL_MS 2000

Timer timerMotion, timerJoystic, timerWifi, timerWebSocket;

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);
MotionController motionController(&motorR, &motorL, &encoderR, &encoderL, &timerMotion);

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(&joystic, &timerJoystic, JOYSTIC_DEADZONE);

CompassBMM150 compass;

WifiController wifiController(&timerWifi);

static AsyncWebServer server(WEBSOCKET_PORT);
static AsyncWebSocket ws("/ws");

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

void initWifi() {
    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!wifiController.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, WIFI_CHECK_MS)) {/*...*/}
    if (wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {/*...*/}
}

void onWSEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
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
            if (info->final && info->index == 0 && info->len == len) {
                if (info->opcode == WS_TEXT) {
                    data[len] = 0;
                    Serial.printf("%s\n", (char *)data);
                    client->ping();
                }
            }
            break;
    }
}

void initWS() {
    ws.onEvent(onWSEvent);

    server.addHandler(&ws).addMiddleware([] (AsyncWebServerRequest *request, ArMiddlewareNext next) {
        if (ws.count() >= WEBSOCKET_MAX_CLIENTS)
            request->send(503, "text/plain", "Server is busy");
        else
            next();
    });
    server.begin();
    timerWebSocket.setTimeout(WEBSOCKET_INTERVAL_MS);
    timerWebSocket.start();
}

void setup() {
    Serial.begin(115200);

    motionController.init(onRightEncoder, onLeftEncoder, MOTOR_PWM_FREQ, MOTOR_PWM_RES);

    joysticController.init(JOYSTIC_INTERVAL_MS);

    if (!compass.init()) {/*...*/}

    initWifi();

    initWS();
}

void loop() {
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)) {
        if(joysticController.isInDeadzone(vert, horz)) {
            motionController.stop();
        } else {
            motionController.move(vert, horz);
        }
    }

    float degree = compass.getCompassDegree();

    if (wifiController.tick()) {/*...*/}

    if(timerWebSocket.tick()) {
        ws.printfAll("value: %.4f", (10.0 / 3.0));
        // ws.textAll("value: 3.3333"); 
        Serial.printf("Clients: %u/%u\n", ws.count(), ws.getClients().size());
        ws.cleanupClients(WEBSOCKET_MAX_CLIENTS);
        timerWebSocket.refresh();
    }
}
