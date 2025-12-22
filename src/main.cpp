#include <Arduino.h>
#include "MotionController/MotionController.h"

// #define DEBUG_ENABLED
// #define COMPASS_ENABLED
// #define JOYSTIC_ENABLED
// #define WIFI_ENABLED

#define ENCODER_R_PIN       32
#define ENCODER_L_PIN       33

#define MOTOR_R_PWM_PIN     25
#define MOTOR_R_FWD_PIN     18
#define MOTOR_R_BCK_PIN     19

#define MOTOR_L_PWM_PIN     26
#define MOTOR_L_FWD_PIN     27
#define MOTOR_L_BCK_PIN     14

#define MOTOR_R_PWM_CHANNEL 0
#define MOTOR_L_PWM_CHANNEL 1

#define MOTORS_PWM_FREQ     1000
#define MOTORS_PWM_RES      8

#define MOTOR_L_CORR        1.0395f

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);
Timer timerMotion;
MotionController motionController(&motorR, &motorL, &encoderR, &encoderL, &timerMotion);

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

#ifdef COMPASS_ENABLED
#include <CompassBMM150.h>
CompassBMM150 compass;
#endif

#ifdef WIFI_ENABLED
#include <WifiController/WifiController.h>
#include <wifi_creds.h>
#define WIFI_CHECK_MS       3000
Timer timerWifi;
WifiController wifiController(&timerWifi);
#endif

#ifdef JOYSTIC_ENABLED
#include "AnalogJoysticController/AnalogJoysticController.h"
#define JOYSTIC_VERT_PIN    34
#define JOYSTIC_HORZ_PIN    35
#define JOYSTIC_DEADZONE    7
#define JOYSTIC_INTERVAL_MS 20
AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN);
Timer timerJoystic;
AnalogJoysticController joysticController(&joystic, &timerJoystic, JOYSTIC_DEADZONE);
#endif

void setup() {
#ifdef DEBUG_ENABLED
    Serial.begin(115200);
#endif

    motionController.init(onRightEncoder, onLeftEncoder, MOTORS_PWM_FREQ, MOTORS_PWM_RES);

#ifdef COMPASS_ENABLED
    if (!compass.init()) {
#ifdef DEBUG_ENABLED
        Serial.println("Compass failed to init");
#endif
    }
#endif

#ifdef JOYSTIC_ENABLED
    joysticController.init(JOYSTIC_INTERVAL_MS);
#endif

#ifdef WIFI_ENABLED
    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!wifiController.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, WIFI_CHECK_MS)) {
#ifdef DEBUG_ENABLED
        Serial.println("Wifi init failed");
#endif
    }
    if (wifiController.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
#ifdef DEBUG_ENABLED
        Serial.println("Wifi connect failed");
#endif
    }
#endif
}

void loop() {
#ifdef JOYSTIC_ENABLED
    int16_t vert, horz;
    if(joysticController.tick(vert, horz)) {
        if(joysticController.isInDeadzone(vert, horz)) {
            motionController.stop();
        } else {
            motionController.move(vert, horz);
        }
    }
#endif

#ifdef COMPASS_ENABLED
    float degree = compass.getCompassDegree();
#ifdef DEBUG_ENABLED
    Serial.print("Compass Degree: "); Serial.println(degree);
#endif
#endif

#ifdef WIFI_ENABLED
    if (wifiController.tick()) {
#ifdef DEBUG_ENABLED
        Serial.println("Wifi check passed successfully");
#endif
    }
#endif
}
