#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <CompassBMM150.h>
#include "DriveController/DriveController.h"
#include "AnalogJoysticController/AnalogJoysticController.h"
#include "WifiController/WifiController.h"
#include <wifi_creds.h>

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

#define MOTOR_R_CORR            1
#define MOTOR_L_CORR            1.0395f

#define ENCODER_R_PIN           32
#define ENCODER_L_PIN           33

#define JOYSTIC_VERT_PIN        34
#define JOYSTIC_HORZ_PIN        35
#define JOYSTIC_DEADZONE        112
#define JOYSTIC_INTERVAL_MS     20

#define WIFI_INTERVAL_MS        3000

CompassBMM150 compass;

Timer timerDrive;
Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);
Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL, MOTOR_R_CORR);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
DriveController driveController(motorRight, motorLeft, encoderRight, encoderLeft, timerDrive);

Timer timerJoystic;
AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticController joysticController(joystic, timerJoystic, JOYSTIC_DEADZONE);

Timer timerWifi;
WifiController wifiController(&timerWifi);

void IRAM_ATTR onRightEncoder() { encoderRight.tick(); }
void IRAM_ATTR onLeftEncoder() { encoderLeft.tick(); }

void createMotionStatus(JsonObject &motion) {
    motion["heading"] = compass.getCompassDegree();
    motion["driving"] = driveController.isDriving();
    if (driveController.isDriving())
        motion["distance"] = driveController.getDistanceMeters();
}

void createHeapStatus(JsonObject &heap) {
    heap["free"] = ESP.getFreeHeap();
    heap["total"] = ESP.getHeapSize();
    heap["maxAlloc"] = ESP.getMaxAllocHeap();
}

void createWifiStatus(JsonObject &wifi) {
    bool connected = wifiController.isConnected();
    wifi["connected"] = connected;
    if (connected)
        wifi["ip"] = wifiController.getIP();
        wifi["rssi"] = wifiController.getRSSI();
}

void createStatus(JsonDocument &doc) {
    doc["type"] = "status";
    doc["uptime"] = millis();
    JsonObject heap = doc["heap"].to<JsonObject>();
    createHeapStatus(heap);
    JsonObject motion = doc["motion"].to<JsonObject>();
    createMotionStatus(motion);
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    createWifiStatus(wifi);
}

void printStatus() {
    StaticJsonDocument<512> status;
    createStatus(status);
    serializeJson(status, Serial);
    Serial.println();
}

void setup() {
    Serial.begin(115200);

    if (!compass.init()) {/*...*/}

    driveController.init(onRightEncoder, onLeftEncoder, MOTOR_PWM_FREQ, MOTOR_PWM_RES);

    // joysticController.init(JOYSTIC_INTERVAL_MS);

    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!wifiController.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, WIFI_INTERVAL_MS)
            || wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {/*...*/}

    printStatus();
}

void loop() {
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)
            && (vert != 0 || horz != 0 || driveController.isDriving()))
        driveController.driveDifferential(vert, horz);

    driveController.tick();

    if (wifiController.tick()) {/*...*/}
}
