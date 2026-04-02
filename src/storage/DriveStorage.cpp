#include "DriveStorage.h"

#define PREFS_DRIVE_CFG     "drive_cfg"
#define PREFS_FREQ_KEY      "freq"
#define PREFS_RES_KEY       "res"
#define PREFS_WHEEL_D_KEY   "wheel_d"
#define PREFS_CIRC_CORR_KEY "circ_corr"
#define PREFS_ENC_SLOTS_KEY "enc_slots"
#define PREFS_MR_MIN_KEY    "mr_min"
#define PREFS_ML_MIN_KEY    "ml_min"

void DriveStorage::begin() { _prefs.begin(PREFS_DRIVE_CFG, false); }

void DriveStorage::reset() { _prefs.clear(); }

void DriveStorage::loadConfig(DriveConfig &target) {
    target.frequency = _prefs.getULong(PREFS_FREQ_KEY, DriveDefaults::frequency);
    target.resolution = _prefs.getUChar(PREFS_RES_KEY, DriveDefaults::resolution);
    target.wheelDiameter = _prefs.getFloat(PREFS_WHEEL_D_KEY, DriveDefaults::wheelDiameter);
    target.circCorrection = _prefs.getFloat(PREFS_CIRC_CORR_KEY, DriveDefaults::circCorrection);
    target.encoderSlots = _prefs.getUChar(PREFS_ENC_SLOTS_KEY, DriveDefaults::encoderSlots);
    target.motorRightMinPWM = _prefs.getUChar(PREFS_MR_MIN_KEY, DriveDefaults::motorRightMinPWM);
    target.motorLeftMinPWM = _prefs.getUChar(PREFS_ML_MIN_KEY, DriveDefaults::motorLeftMinPWM);
}

void DriveStorage::saveConfig(const DriveConfig &cfg) {
    saveFrequency(cfg.frequency);
    saveResolution(cfg.resolution);
    saveWheelDiameter(cfg.wheelDiameter);
    saveCircCorrection(cfg.circCorrection);
    saveEncoderSlots(cfg.encoderSlots);
    saveMotorRightMinPWM(cfg.motorRightMinPWM);
    saveMotorLeftMinPWM(cfg.motorLeftMinPWM);
}

void DriveStorage::saveFrequency(uint32_t freq) { _prefs.putULong(PREFS_FREQ_KEY, freq); }

void DriveStorage::saveResolution(uint8_t res) { _prefs.putUChar(PREFS_RES_KEY, res); }

void DriveStorage::saveWheelDiameter(double diameter) { _prefs.putFloat(PREFS_WHEEL_D_KEY, diameter); }

void DriveStorage::saveCircCorrection(double correction) { _prefs.putFloat(PREFS_CIRC_CORR_KEY, correction); }

void DriveStorage::saveEncoderSlots(uint8_t slots) { _prefs.putUChar(PREFS_ENC_SLOTS_KEY, slots); }

void DriveStorage::saveMotorRightMinPWM(uint8_t pwm) { _prefs.putUChar(PREFS_MR_MIN_KEY, pwm); }

void DriveStorage::saveMotorLeftMinPWM(uint8_t pwm) { _prefs.putUChar(PREFS_ML_MIN_KEY, pwm); }
