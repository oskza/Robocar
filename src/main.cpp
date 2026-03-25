#include <Arduino.h>
#include "controllers/AnalogJoysticController.h"
#include "controllers/CompassController.h"
#include "controllers/DriveController.h"
#include "controllers/NetworkController.h"
#include "controllers/WebSocketController.h"
#include "storage/DeviceStorage.h"
#include <RGBIndicator.h>
#include <PushBtn.h>
#include <INA226.h>

#ifndef MONITOR_SPEED
#define MONITOR_SPEED           115200
#endif

#define MOTOR_R_PWM_CHANNEL     0
#define MOTOR_L_PWM_CHANNEL     1

#define MOTOR_R_PWM_PIN         25
#define MOTOR_R_NORM_PIN        19
#define MOTOR_R_REV_PIN         18

#define MOTOR_L_PWM_PIN         26
#define MOTOR_L_NORM_PIN        14
#define MOTOR_L_REV_PIN         27

#define ENCODER_R_PIN           32
#define ENCODER_L_PIN           33

#define JOYSTIC_VERT_PIN        34
#define JOYSTIC_HORZ_PIN        35

#define INDIC_R_CHANNEL         2
#define INDIC_G_CHANNEL         3
#define INDIC_B_CHANNEL         4

#define INDIC_R_PIN             2
#define INDIC_G_PIN             4
#define INDIC_B_PIN             16

#define INDIC_FREQ              5000
#define INDIC_RES               8

#define BTN_OPTIONS_PIN         17

#define INA_I2C_ADDRESS         0x40
#define INA_MAX_CURRENT         0.8
#define INA_SHUNT_OHM           0.1

#define SERVER_PORT             80
#define WEBSOCKET_PATH          "/ws"

Motor motorRight(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorLeft(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);
Encoder encoderRight(ENCODER_R_PIN);
Encoder encoderLeft(ENCODER_L_PIN);
DriveStorage driveStorage;
Timer driveTimer;
StopWatch driveStopwatch;
DriveController driveController(motorRight, motorLeft, encoderRight, encoderLeft, 
                                    driveStorage, driveTimer, driveStopwatch);

DFRobot_BMM150_I2C compass(&Wire, I2C_ADDRESS_4);
CompassController compassController(compass);

NetworkStorage networkStorage;
Timer networkTimer;
NetworkController networkController(networkStorage, networkTimer);

AsyncWebSocket ws(WEBSOCKET_PATH);
WebSocketStorage wsStorage;
Timer wsTimer;
WebSocketController wsController(ws, wsStorage, wsTimer);

AsyncWebServer server(SERVER_PORT);

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
AnalogJoysticStorage joysticStorage;
Timer joysticTimer;
AnalogJoysticController joysticController(joystic, joysticStorage, joysticTimer);

Timer btnOptionsTimer;
PushBtn btnOptions(btnOptionsTimer, BTN_OPTIONS_PIN);

INA226 ina(INA_I2C_ADDRESS);

RGBIndicator indic(INDIC_R_PIN, INDIC_G_PIN, INDIC_B_PIN, 
                    INDIC_R_CHANNEL, INDIC_G_CHANNEL, INDIC_B_CHANNEL);

DeviceStorage deviceStorage;
Timer statusReportTimer;

void getHeapMetrics(JsonObject &target) {
    target["free"] = ESP.getFreeHeap();
    target["total"] = ESP.getHeapSize();
    target["maxAlloc"] = ESP.getMaxAllocHeap();
}

/** TODO: add remaining meters/ms/degree */
void getDriveStatus(JsonObject &target) {
    bool driving = driveController.isDriving();
    target["mode"] = driveController.getMode();
    target["driving"] = driving;
    if (driving) {
        target["pwmRight"] = driveController.getRightPWM();
        target["pwmLeft"] = driveController.getLeftPWM();
        target["distance"] = driveController.getDistanceMeters();
        target["duration"] = driveController.getDurationMs();
    }
}

void getNetworkStatus(JsonObject &target) {
    bool connected = networkController.isConnected();
    target["connected"] = connected;
    if (connected)
        target["rssi"] = networkController.getRSSI();
}

void createStatus(JsonDocument &doc) {
    doc["type"] = "status";
    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["uptime"] = millis();
    payload["voltage"] = ina.getBusVoltage();
    payload["clients"] = wsController.getClientsCount();
    payload["heading"] = compassController.getHeading();
    JsonObject heap = payload["heap"].to<JsonObject>();
    getHeapMetrics(heap);
    JsonObject drive = payload["drive"].to<JsonObject>();
    getDriveStatus(drive);
    JsonObject network = payload["network"].to<JsonObject>();
    getNetworkStatus(network);
}

void handleAutoDrive(JsonObject &payload) {
    const char *navigation = payload["navigation"] | "";
    if (!navigation)
        return;
    if (strcmp(navigation, "duration") == 0) {
        int16_t velocity = payload["velocity"] | 0;
        int16_t turn = payload["turn"] | 0;
        uint32_t ms = payload["duration"] | 0;
        driveController.driveFor(velocity, turn, ms);
        return;
    }
    if (strcmp(navigation, "distance") == 0) {
        int16_t velocity = payload["velocity"] | 0;
        double meters = payload["distance"] | 0.00f;
        driveController.driveDistance(velocity, meters);
        return;
    }
}

void handleManualDrive(JsonObject &payload) {
    int16_t velocity = payload["velocity"] | 0;
    int16_t turn = payload["turn"] | 0;
    const char *control = payload["control"] | "";
    if (!control)
        return;
    if (strcmp(control, "joystic") == 0) {
        driveController.driveDifferential(velocity, turn);
        return;
    }
    if (strcmp(control, "keyboard") == 0) {
        bool up = payload["up"] | false;
        bool down = payload["down"] | false;
        bool right = payload["right"] | false;
        bool left = payload["left"] | false;
        driveController.driveDiscreteArcade(velocity, turn, up, down, right, left);
    }
}

void handleCommand(JsonDocument &doc) {
    const char *cmd = doc["cmd"] | "";
    if (!cmd) 
        return;
    if (strcmp(cmd, "set-mode") == 0) {
        const char *mode = doc["payload"] | "";
        driveController.setMode(mode);
        return;
    }
    if (strcmp(cmd, "stop") == 0) {
        driveController.stop();
        return;
    }
    if (strcmp(cmd, "drive") == 0) {
        JsonObject payload = doc["payload"];
        if (!payload) 
            return;
        if(driveController.isModeManual()) {
            handleManualDrive(payload);
            return;
        }
        handleAutoDrive(payload);
    }
}

void setup() {
    Serial.begin(MONITOR_SPEED);

    Wire.begin();
    if (!ina.begin() 
            || !ina.setAverage(INA226_16_SAMPLES)
            || ina.setMaxCurrentShunt(INA_MAX_CURRENT, INA_SHUNT_OHM) != 0) {
        Serial.print("INA Init Error");
    }

    driveController.init();

    if (!compassController.init()) {
        Serial.print("BMM150 Init Error: ");
        Serial.print(compassController.getError());
    }

    /** TODO: init joystic when connected */
    // joysticController.init();

    btnOptions.init();

    if (networkController.init()) {
        networkController.connect() != WL_CONNECTED;
        wsController.init(server, handleCommand);
        server.begin();
    } else {
        Serial.print("Wifi Init Error");
    }

    deviceStorage.begin();
    indic.init(INDIC_FREQ, INDIC_RES, deviceStorage.loadIndicatorIntensity());

    statusReportTimer.setTimeout(deviceStorage.loadStatusReportIntervalMs());
    statusReportTimer.start();

    indic.waiting();
}

void loop(void) {
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)
            && (vert != 0 || horz != 0 || driveController.isDriving()))
        driveController.driveDifferential(vert, horz);
    else
        driveController.tick();

    if (networkController.tick()) {/*...*/}

    wsController.tick();

    btnOptions.tick();

    if (statusReportTimer.tick()) {
        statusReportTimer.refresh();
        if (wsController.hasClients()) {
            StaticJsonDocument<512> status;
            createStatus(status);
            wsController.sendAll(status);
        }
    }
}
