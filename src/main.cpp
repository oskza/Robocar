#include <Arduino.h>
#include <WebSocketServer.h>
#include "RobocarApp.h"
#include "command/CommandDispatcher.h"
#include "command/CommandProcessor.h"
#include "hardware/RobotHardwareConfig.h"
#include "motion/MotionStorage.h"
#include "network/WifiStorage.h"
#include "ota/OtaService.h"
#include "robot/Robot.h"
#include "robot/RobotStorage.h"
#include "telemetry/TelemetryService.h"
#include "websocket/WebSocketService.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED 115200
#endif

namespace {
    constexpr uint16_t WS_PORT = 80;
    constexpr char WS_PATH[] = "/ws";
    constexpr uint32_t WS_BROADCAST_INTERVAL_MS = 3000;

    using namespace RobotHardwareConfig;

    Ina226PowerMonitor powerMonitor(INA226_ADDRESS);
    PowerService powerService(powerMonitor);

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

    WheelActuator leftWheel(leftMotor);
    WheelActuator rightWheel(rightMotor);

    DifferentialDrive differentialDrive(leftWheel, rightWheel);

    Encoder leftEncoder(ENCODER_L_PIN);
    Encoder rightEncoder(ENCODER_R_PIN);

    Odometry odometry(leftEncoder, rightEncoder);

    Bmm150Compass compass;

    MotionController motionController(differentialDrive, odometry, compass);

    MotionStorage motionStorage;
    RobotStorage robotStorage;

    Robot robot(powerService, wifiController, motionController);

    CommandDispatcher commandDispatcher(
        robot,
        robotStorage,
        motionController,
        motionStorage,
        wifiController,
        wifiStorage
    );

    CommandProcessor commandProcessor(commandDispatcher);

    WebSocketServer webSocketServer(WS_PORT, WS_PATH);

    WebSocketService webSocketService(webSocketServer, commandProcessor);

    TelemetryService telemetryService(robot, webSocketServer, WS_BROADCAST_INTERVAL_MS);

    OtaService otaService;

    RobocarApp app(
        robot,
        robotStorage,
        motionStorage,
        wifiStorage,
        otaService,
        webSocketService,
        telemetryService
    );

    void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

    void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }
}

void setup() {
    Serial.begin(MONITOR_SPEED);
    leftEncoder.begin(onLeftEncoder);
    rightEncoder.begin(onRightEncoder);
    if (!app.begin())
        Serial.println("Application started with errors");
}

void loop() { app.update(millis()); }
