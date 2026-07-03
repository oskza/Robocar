#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Esp.h>
#include <WifiManager.h>
#include <PrivateConfig.h>
#include <WebSocketServer.h>
#include <Bmm150Compass.h>
#include <Ina226PowerMonitor.h>
#include "app/SystemCommandHandler.h"
#include "domain/system/SystemCommand.h"
#include "domain/motion/MotionController.h"
#include "domain/RobotSnapshot.h"
#include "telemetry/WebSocketTelemetry.h"
#include "app/MotionCommandHandler.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED               115200
#endif

#define MOTOR_R_PWM_PIN             25
#define MOTOR_R_NORM_PIN            19
#define MOTOR_R_REV_PIN             18
#define MOTOR_R_PWM_CHANNEL         0
#define MOTOR_R_MIN_PWM             110

#define MOTOR_L_PWM_PIN             26
#define MOTOR_L_NORM_PIN            14
#define MOTOR_L_REV_PIN             27
#define MOTOR_L_PWM_CHANNEL         1
#define MOTOR_L_MIN_PWM             110

#define MOTOR_PWM_FREQ              1000

#define WHEEL_ACCELERATION          5
#define WHEEL_DIAMETER              0.067f
#define WHEEL_CIRCUMFERENCE_FACTOR  1.0f

#define ENCODER_R_PIN               32
#define ENCODER_L_PIN               33

#define ENCODER_SLOTS               20

#define HEADING_TOLERANCE_DEGREES   12.0f

#define INA226_ADDRESS              0x40
#define INA226_MAX_CURRENT_AMPS     0.8f
#define INA226_SHUNT_OHMS           0.1f

#define WS_PORT                     80
#define WS_PATH                     "/ws"

#define HOSTNAME                    "robocar"

#define MOTION_UPDATE_INTERVAL_MS   50
#define WIFI_UPDATE_INTERVAL_MS     3000
#define WS_UPDATE_INTERVAL_MS       100
#define WS_BROADCAST_INTERVAL_MS    3000

WifiManager wifi;

WebSocketServer webSocketServer(WS_PORT, WS_PATH);

MotorDriver rightMotor(MOTOR_R_PWM_PIN, MOTOR_R_NORM_PIN, MOTOR_R_REV_PIN, MOTOR_R_PWM_CHANNEL);
MotorDriver leftMotor(MOTOR_L_PWM_PIN, MOTOR_L_NORM_PIN, MOTOR_L_REV_PIN, MOTOR_L_PWM_CHANNEL);

Encoder rightEncoder(ENCODER_R_PIN);
Encoder leftEncoder(ENCODER_L_PIN);

Bmm150Compass compass;

Ina226PowerMonitor powerMonitor(INA226_ADDRESS);

WheelOutputController rightWheel(rightMotor);
WheelOutputController leftWheel(leftMotor);

DifferentialDrive differential(rightWheel, leftWheel);
Odometry odometry(rightEncoder, leftEncoder);
MotionController motion(differential, odometry, compass);

static float wheelCircumference(float diameter, float factor = 1.0f) { return diameter * PI * factor; }

static RobotSnapshot createSnapshot(uint32_t uptimeMs) {
    RobotSnapshot snapshot;
    snapshot.uptimeMs = uptimeMs;
    snapshot.network.connected = wifi.isConnected();
    snapshot.network.rssi = wifi.getRssi();
    snapshot.network.localIp = wifi.getLocalIp();
    snapshot.power.connected = powerMonitor.isConnected();
    snapshot.power.busVoltage = powerMonitor.getBusVoltage();
    snapshot.power.currentMilliamps = powerMonitor.getCurrentMa();
    snapshot.power.powerMilliwatts = powerMonitor.getPowerMw();
    snapshot.motion = motion.getSnapshot();
    snapshot.odometry.distanceMeters = odometry.getMeters();
    snapshot.odometry.averageTicks = odometry.getTicks();
    return snapshot;
}

WebSocketTelemetry telemetry(webSocketServer, createSnapshot);

static MotionCommandHandler motionHandler([](const MotionCommand &command) { motion.execute(command); });

static SystemCommandHandler systemHandler([](const SystemCommand &command) {
    switch (command.type) {
        case SystemCommandType::PING:
            break;
        case SystemCommandType::RESTART:
            ESP.restart();
            break;
        case SystemCommandType::FACTORY_RESET:
            /** TODO: Reset persisted config */
            ESP.restart();
            break;
    }
});

uint32_t lastWifiUpdateMs = 0;
uint32_t lastMotionUpdateMs = 0;
uint32_t lastWsUpdateMs = 0;
uint32_t lastWsBroadcastMs = 0;

void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }
void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

void setup() {
    if (!wifi.begin(WIFI_SSID, WIFI_PASSWORD)) {}

    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.begin();

    webSocketServer.begin([](const char *data, size_t len) {
        if (motionHandler.handle(data, len))
            return;
        systemHandler.handle(data, len);
    });

    rightMotor.begin(MOTOR_PWM_FREQ, MOTOR_R_MIN_PWM);
    leftMotor.begin(MOTOR_PWM_FREQ, MOTOR_L_MIN_PWM);

    rightEncoder.begin(onRightEncoder);
    leftEncoder.begin(onLeftEncoder);

    odometry.begin(wheelCircumference(WHEEL_DIAMETER, WHEEL_CIRCUMFERENCE_FACTOR), ENCODER_SLOTS);

    if (!compass.begin()) {}

    if (!powerMonitor.begin(INA226_MAX_CURRENT_AMPS, INA226_SHUNT_OHMS)) {}

    motion.begin(WHEEL_ACCELERATION, HEADING_TOLERANCE_DEGREES);
}

void loop() {
    uint32_t now = millis();

    ArduinoOTA.handle();

    if (now - lastMotionUpdateMs >= MOTION_UPDATE_INTERVAL_MS) {
        lastMotionUpdateMs = now;
        motion.update(now);
    }

    if (now - lastWifiUpdateMs >= WIFI_UPDATE_INTERVAL_MS) {
        lastWifiUpdateMs = now;
        wifi.update();
    }

    if (now - lastWsUpdateMs >= WS_UPDATE_INTERVAL_MS) {
        lastWsUpdateMs = now;
        webSocketServer.update();
    }

    if (now - lastWsBroadcastMs >= WS_BROADCAST_INTERVAL_MS) {
        lastWsBroadcastMs = now;
        telemetry.update(now);
    }
}
