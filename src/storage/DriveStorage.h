#ifndef DRIVE_STORAGE_H
#define DRIVE_STORAGE_H
#include <Preferences.h>
#include "config/DriveConfig.h"
#include "defaults/DriveDefaults.h"

class DriveStorage {
private:
    Preferences _prefs;
public:
    void begin();
    void reset();
    void loadConfig(DriveConfig &target);
    void saveConfig(const DriveConfig &cfg);
    void saveFrequency(uint32_t freq);
    void saveResolution(uint8_t res);
    void saveWheelDiameter(double diameter);
    void saveCircCorrection(double correction);
    void saveEncoderSlots(uint8_t slots);
    void saveMotorRightMinPWM(uint8_t pwm);
    void saveMotorLeftMinPWM(uint8_t pwm);
};
#endif
