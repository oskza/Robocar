#include <Arduino.h>
#include <WebSocketServer.h>
#include "app/ota/OtaService.h"
#include "app/websocket/WebSocketService.h"
#include "app/telemetry/TelemetryService.h"
#include "app/command/CommandProcessor.h"
#include "hardware/RobotHardwareConfig.h"
#include "robot/Robot.h"

#ifndef MONITOR_SPEED
#define MONITOR_SPEED 115200
#endif

#define WS_PORT 80
#define WS_PATH "/ws"

#define WS_BROADCAST_INTERVAL_MS 3000

using namespace RobotHardwareConfig;

Ina226PowerMonitor powerMonitor(INA226_ADDRESS);
PowerService powerService(powerMonitor);

WifiController wifiController;

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

WifiStorage wifiStorage;
MotionStorage motionStorage;
RobotStorage robotStorage;

Robot robot(
    powerService,
    wifiController,
    motionController,
    wifiStorage,
    motionStorage,
    robotStorage
);

WebSocketServer webSocketServer(WS_PORT, WS_PATH);

OtaService otaService;

CommandProcessor commandProcessor(robot);

WebSocketService webSocketService(webSocketServer, commandProcessor);

TelemetryService telemetryService(robot, webSocketServer, WS_BROADCAST_INTERVAL_MS);

void IRAM_ATTR onLeftEncoder() { leftEncoder.tick(); }

void IRAM_ATTR onRightEncoder() { rightEncoder.tick(); }

void setup() {
    leftEncoder.begin(onLeftEncoder);
    rightEncoder.begin(onRightEncoder);
    robot.begin(
        MOTOR_PWM_FREQ,
        ENCODER_SLOTS,
        INA226_MAX_CURRENT_AMPS,
        INA226_SHUNT_OHMS
    );
    otaService.begin(robot.getHostname());
    webSocketService.begin();
}

void loop() {
    otaService.update();
    robot.update();
    webSocketService.update();
    if (robot.isTelemetryEnabled())
        telemetryService.update(robot.getUptimeMs());
}
