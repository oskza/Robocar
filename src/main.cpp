#include <Arduino.h>
#include <MotionController.h>

#define DEBUG_ENABLED
#define COMPASS_ENABLED
#define JOYSTIC_ENABLED
#define WIFI_ENABLED

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

#ifdef COMPASS_ENABLED
#include <CompassBMM150.h>
CompassBMM150 compass;
#endif

#ifdef WIFI_ENABLED
#include <WifiManager.h>
#include <wifi_creds.h>
#define WIFI_CHECK_MS       3000
Timer wifiTimer;
WifiManager wifiManager(&wifiTimer);
#endif

#ifdef JOYSTIC_ENABLED
#include <AnalogJoystic.h>
#define JOYSTIC_HORZ_PIN    35
#define JOYSTIC_VERT_PIN    34
#define JOYSTIC_DEADZONE    7
AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN, JOYSTIC_DEADZONE);
#endif

Motor motorR(MOTOR_R_PWM_PIN, MOTOR_R_FWD_PIN, MOTOR_R_BCK_PIN, MOTOR_R_PWM_CHANNEL);
Motor motorL(MOTOR_L_PWM_PIN, MOTOR_L_FWD_PIN, MOTOR_L_BCK_PIN, MOTOR_L_PWM_CHANNEL, MOTOR_L_CORR);
Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);
Timer motionTimer;
MotionController motionController(&motorR, &motorL, &encoderR, &encoderL, &motionTimer);

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

#ifdef JOYSTIC_ENABLED
void joysticHandle() {
    int16_t vert = joystic.readVerticalPWM();
    int16_t horz = joystic.readHorizontalPWM();
    if(joystic.isInDeadzone(vert, horz)) {
        motionController.stop();
        return;
    }
    motionController.move(vert, horz);
}
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
    joystic.calibrateCenter();
#endif

#ifdef WIFI_ENABLED
    IPAddress localIP(LOCAL_IP_1, LOCAL_IP_2, LOCAL_IP_3, LOCAL_IP_4);
    IPAddress gateway(GATEWAY_1, GATEWAY_2, GATEWAY_3, GATEWAY_4);
    IPAddress subnet(SUBNET_1, SUBNET_2, SUBNET_3, SUBNET_4);
    IPAddress primaryDNS(PRIMARY_DNS_1, PRIMARY_DNS_2, PRIMARY_DNS_3, PRIMARY_DNS_4);
    IPAddress secondaryDNS(SECONDARY_DNS_1, SECONDARY_DNS_2, SECONDARY_DNS_3, SECONDARY_DNS_4);
    if (!wifiManager.init(localIP, gateway, subnet, primaryDNS, secondaryDNS, WIFI_CHECK_MS)) {
#ifdef DEBUG_ENABLED
        Serial.println("Wifi init failed");
#endif
    }
    if (wifiManager.connect(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
#ifdef DEBUG_ENABLED
        Serial.println("Wifi connect failed");
#endif
    }
#endif
}

void loop() {
    static bool isMoving = true;
    if (isMoving 
        // && motionController.moveForMs(180, 0, 3000)
        && motionController.moveForMeters(180, 0, 1.20)
    ) { isMoving = false; }

#ifdef JOYSTIC_ENABLED
    joysticHandle();
#endif

#ifdef WIFI_ENABLED
    if (wifiManager.tick()) {/*...*/}
#endif
}
