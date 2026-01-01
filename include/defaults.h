#ifndef DEFAULTS_H
#define DEFAULTS_H

#define MOTOR_PWM_FREQ              1000
#define MOTOR_PWM_RES               8

#define MOTOR_R_PWM_CHANNEL         0
#define MOTOR_L_PWM_CHANNEL         1

#define MOTOR_R_PWM_PIN             25
#define MOTOR_R_FWD_PIN             18
#define MOTOR_R_BCK_PIN             19

#define MOTOR_L_PWM_PIN             26
#define MOTOR_L_FWD_PIN             27
#define MOTOR_L_BCK_PIN             14

#define ENCODER_R_PIN               32
#define ENCODER_L_PIN               33

#define JOYSTIC_VERT_PIN            34
#define JOYSTIC_HORZ_PIN            35

#define WHEEL_DIAMETER              0.065

#define MOTOR_MIN_PWM               110

#define MOTOR_R_CORR                1
#define MOTOR_L_CORR                1.0395f

#define ENCODER_SLOTS               20

#define JOYSTIC_DEADZONE            112

#define SERVER_PORT                 80
#define WEBSOCKET_PATH              "/ws"
#define WEBSOCKET_MAX_CLIENTS       2

#define STATUS_REPORT_INTERVAL_MS   5000
#define WIFI_INTERVAL_MS            3000
#define WEBSOCKET_INTERVAL_MS       2000
#define JOYSTIC_INTERVAL_MS         20

#endif
