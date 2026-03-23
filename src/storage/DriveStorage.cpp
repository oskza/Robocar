#include "DriveStorage.h"

void DriveStorage::begin() { _prefs.begin("drive_cfg", false); }

void DriveStorage::reset() { _prefs.clear(); }

void DriveStorage::loadConfig(DriveConfig &target) {
    target.frequency = _prefs.getULong("freq", DriveDefaults::frequency);
    target.resolution = _prefs.getUChar("res", DriveDefaults::resolution);
    target.wheelDiameter = _prefs.getFloat("wheel_d", DriveDefaults::wheelDiameter);
    target.encoderSlots = _prefs.getUChar("enc_slots", DriveDefaults::encoderSlots);
    target.motorRightMinPWM = _prefs.getUChar("mr_min", DriveDefaults::motorRightMinPWM);;
    target.motorLeftMinPWM = _prefs.getUChar("ml_min", DriveDefaults::motorLeftMinPWM);
}

void DriveStorage::saveConfig(const DriveConfig &cfg) {
    saveFrequency(cfg.frequency);
    saveResolution(cfg.resolution);
    saveWheelDiameter(cfg.wheelDiameter);
    saveEncoderSlots(cfg.encoderSlots);
    saveMotorRightMinPWM(cfg.motorRightMinPWM);
    saveMotorLeftMinPWM(cfg.motorLeftMinPWM);
}

void DriveStorage::saveFrequency(uint32_t freq) { _prefs.putULong("freq", freq); }

void DriveStorage::saveResolution(uint8_t res) { _prefs.putUChar("res", res); }

void DriveStorage::saveWheelDiameter(double diameter) { _prefs.putFloat("wheel_d", diameter); }

void DriveStorage::saveEncoderSlots(uint8_t slots) { _prefs.putUChar("enc_slots", slots); }

void DriveStorage::saveMotorRightMinPWM(uint8_t pwm) { _prefs.putUChar("mr_min", pwm); }

void DriveStorage::saveMotorLeftMinPWM(uint8_t pwm) { _prefs.putUChar("ml_min", pwm); }
