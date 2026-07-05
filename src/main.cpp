#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WebSocketServer.h>
#include "robot/Robot.h"
#include "hardware/RobotHardwareConfig.h"

using namespace RobotHardwareConfig;

#ifndef MONITOR_SPEED
#define MONITOR_SPEED           115200
#endif

#define WS_PORT                 80
#define WS_PATH                 "/ws"

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

WebSocketServer webSocketServer(WS_PORT, WS_PATH);

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

    ArduinoOTA.setHostname(wifiController.getHostname());
    ArduinoOTA.begin();

    webSocketServer.begin([](const char *data, size_t len) {});
}

void loop() {
    ArduinoOTA.handle();
    robot.update();
    webSocketServer.update();
}
