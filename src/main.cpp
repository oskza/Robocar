#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WebSocketServer.h>
#include "app/command/CommandService.h"
#include "app/websocket/WebSocketService.h"
#include "app/telemetry/TelemetryService.h"
#include "hardware/RobotHardwareConfig.h"
#include "robot/Robot.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED 115200
#endif

#define WS_PORT 80
#define WS_PATH "/ws"

#define WS_BROADCAST_INTERVAL_MS 3000

using namespace RobotHardwareConfig;

SystemController systemController;

Ina226PowerMonitor powerMonitor(INA226_ADDRESS);
PowerController powerController(powerMonitor);

WifiController wifiController;
WifiStorage wifiStorage;

MotorDriver leftMotor(
    MOTOR_L_PWM_PIN,
    MOTOR_L_NORM_PIN,
    MOTOR_L_REV_PIN,
    MOTOR_L_PWM_CHANNEL
);

MotorDriver rightMotor(
    MOTOR_R_PWM_PIN,
    MOTOR_R_NORM_PIN,
    MOTOR_R_REV_PIN,
    MOTOR_R_PWM_CHANNEL
);

WheelOutputController leftWheel(leftMotor);
WheelOutputController rightWheel(rightMotor);

DifferentialDrive differential(leftWheel, rightWheel);

Encoder leftEncoder(ENCODER_L_PIN);
Encoder rightEncoder(ENCODER_R_PIN);

Odometry odometry(leftEncoder, rightEncoder);

Bmm150Compass compass;

MotionController motionController(differential, odometry, compass);

MotionStorage motionStorage;

RobotStorage robotStorage;

Robot robot(
    systemController,
    powerController,
    wifiController,
    wifiStorage,
    motionController,
    motionStorage,
    robotStorage
);

CommandService commandService(robot);

WebSocketServer webSocketServer(WS_PORT, WS_PATH);

WebSocketService webSocketService(webSocketServer, commandService);

TelemetryService telemetryService(robot, webSocketServer, WS_BROADCAST_INTERVAL_MS);

void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }

void setup() {
    Serial.begin(MONITOR_SPEED);
    delay(500);

    leftEncoder.begin(onLeftEncoder);
    rightEncoder.begin(onRightEncoder);

    robot.begin(
        MOTOR_PWM_FREQ,
        ENCODER_SLOTS,
        INA226_MAX_CURRENT_AMPS,
        INA226_SHUNT_OHMS
    );

    ArduinoOTA.setHostname(wifiController.getHostname());
    ArduinoOTA.begin();

    webSocketService.begin();
}

void loop() {
    ArduinoOTA.handle();

    robot.update();

    webSocketService.update();

    telemetryService.update(robot.getUptimeMs());

    static bool printed = false;
    if (!printed && wifiController.isConnected()) {
        printed = true;
        Serial.println(wifiController.getSnapshot().station.ip);
    }
}
