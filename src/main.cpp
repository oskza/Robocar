#include <Arduino.h>
#include "WiFi.h"
#include <MotorsX2.h>
#include <Encoder.h>
#include <CompassBMM150.h>
#include <AnalogJoystic.h>
#include <Timer.h>

#define WHEEL_DIAMETER      0.065
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * 3.14159265359)

#define MOTOR_R_PWM_PIN     25
#define MOTOR_R_FWD_PIN     18
#define MOTOR_R_BCK_PIN     19
#define MOTOR_R_PWM_CHANNEL 0

#define MOTOR_L_PWM_PIN     26
#define MOTOR_L_FWD_PIN     27
#define MOTOR_L_BCK_PIN     14
#define MOTOR_L_PWM_CHANNEL 1

#define MOTOR_L_CORR        1.0395f

#define MOTORS_PWM_FREQ     1000
#define MOTORS_PWM_RES      8

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

#define ENCODER_SLOTS       20

#define JOYSTIC_HORZ_PIN    35
#define JOYSTIC_VERT_PIN    34
#define JOYSTIC_DEADZONE    7

#define LOCAL_IP_1          192
#define LOCAL_IP_2          168
#define LOCAL_IP_3          1
#define LOCAL_IP_4          200

#define GATEWAY_1           192
#define GATEWAY_2           168
#define GATEWAY_3           1
#define GATEWAY_4           1

#define SUBNET_1            255
#define SUBNET_2            255
#define SUBNET_3            0
#define SUBNET_4            0

#define PRIMARY_DNS_1       8
#define PRIMARY_DNS_2       8
#define PRIMARY_DNS_3       8
#define PRIMARY_DNS_4       8

#define SECONDARY_DNS_1     8
#define SECONDARY_DNS_2     8
#define SECONDARY_DNS_3     4
#define SECONDARY_DNS_4     4

#define WIFI_SSID           "dom-24"
#define WIFI_PASSWORD       "11@ZaHero#99"

#define WIFI_CHECK_MS       3000

#define METERS_TO_TICKS(meters) ((uint32_t)round((double)(meters) / WHEEL_CIRCUMFERENCE * ENCODER_SLOTS))
#define TICKS_TO_METERS(ticks) (((double)(ticks) / ENCODER_SLOTS) * WHEEL_CIRCUMFERENCE)

struct WifiManager {
    bool init() {
        IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
        IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
        IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
        IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
        IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
        WiFi.mode(WIFI_STA);
        return WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
    }
    wl_status_t connect() {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        return WiFi.status();
    }
    bool checkConnectivity() {
        return WiFi.status() == WL_CONNECTED || WiFi.reconnect();
    }
};

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
MotorsX2 motors(&motorR, &motorL);

Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);

CompassBMM150 compass;

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN, JOYSTIC_DEADZONE);

Timer timerMotors;
Timer timerWifi(WIFI_CHECK_MS);

WifiManager wifiManager;

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

void joysticHandle() {
    int16_t vert = joystic.readVerticalPWM();
    int16_t horz = joystic.readHorizontalPWM();
    if(joystic.isInDeadzone(vert, horz)) {
        motors.stop();
        return;
    }
    motors.move(vert, horz);
}

bool moveForMs(int16_t velocity, int16_t turn, uint32_t ms) {
    if(timerMotors.tick()) {
        motors.stop();
        timerMotors.reset();
        return true;
    }
    if(!timerMotors.isRunning()) {
        timerMotors.setTimeout(ms);
        timerMotors.start();
    }
    motors.move(velocity, turn);
    return false;
}

bool moveForMeters(int16_t velocity, int16_t turn, float meters) {
    uint32_t ticks = METERS_TO_TICKS(meters);
    if(encoderR.getCount() >= ticks 
            || encoderL.getCount() >= ticks) {
        motors.stop();
        encoderR.reset();
        encoderL.reset();
        return true;
    }
    motors.move(velocity, turn);
    return false;
}

void setup() {
    Serial.begin(115200);

    if (!wifiManager.init() || wifiManager.connect() != WL_CONNECTED) {/*...*/}
    timerWifi.start();

    // motors.init(MOTORS_PWM_FREQ, MOTORS_PWM_RES);

    // encoderR.init(onRightEncoder);
    // encoderL.init(onLeftEncoder);

    // if (!compass.init()) {/*...*/}

    // joystic.calibrateCenter();
}

void loop() {
    // joysticHandle();

    if (timerWifi.tick()) {
        wifiManager.checkConnectivity();
        timerWifi.refresh();
    }
}
