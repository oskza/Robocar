#include "DriveController.h"

DriveController *DriveController::_instance = nullptr;

DriveController::DriveController(Motor &motorRight, Motor &motorLeft, 
                                    Encoder &encoderRight, Encoder &encoderLeft, 
                                    Timer &timer, StopWatch &stopwatch, DriveStorage &storage) 
                                : _motorRight(motorRight), _motorLeft(motorLeft), 
                                    _encoderRight(encoderRight), _encoderLeft(encoderLeft), 
                                    _timer(timer), _stopwatch(stopwatch), _storage(storage), 
                                    _config{}, _mode(DRIVE_MODE_MANUAL), _circumference(0), 
                                    _targetTicks(0) { _instance = this; }

void IRAM_ATTR DriveController::_onRightEncoder() { if (_instance) _instance->_encoderRight.tick(); }

void IRAM_ATTR DriveController::_onLeftEncoder() { if (_instance) _instance->_encoderLeft.tick(); }

const char* DriveController::modeToString(uint8_t mode) {
    return (mode == DRIVE_MODE_AUTO) 
                ? "auto" 
                : (mode == DRIVE_MODE_MANUAL)
                    ? "manual"
                    : "unknown";
}

void DriveController::init(uint8_t mode) {
    _storage.begin();
    _storage.loadConfig(_config);
    _motorRight.init(_config.frequency, _config.resolution, _config.motorRightMinPWM);
    _motorLeft.init(_config.frequency, _config.resolution, _config.motorLeftMinPWM);
    _encoderRight.init(_onRightEncoder);
    _encoderLeft.init(_onLeftEncoder);
    _timer.reset();
    _stopwatch.stop();
    _mode = (mode == DRIVE_MODE_AUTO) ? DRIVE_MODE_AUTO : DRIVE_MODE_MANUAL;
    _circumference = wheelCircumference(_config.wheelDiameter, _config.circCorrection);
    _targetTicks = 0;
}

void DriveController::stop() {
    if (!isDriving())
        return;
    _motorRight.stop(); 
    _motorLeft.stop();
    _encoderRight.reset();
    _encoderLeft.reset();
    _stopwatch.stop();
    if (isModeAuto()) {
        _timer.reset();
        _targetTicks = 0;
    }
}

bool DriveController::tick() {
    if (isModeAuto() && isDriving() 
            && (_timer.tick() 
                || (_targetTicks > 0 && getDistanceTicks() >= _targetTicks))) {
        stop();
        return true;
    }
    return false;
}

/**
 * velocity > 0 : forward 
 * velocity < 0 : backward 
 * turn     > 0 : rotate left (CCW) 
 * turn     < 0 : rotate right (CW) 
*/
void DriveController::driveDifferential(int16_t velocity, int16_t turn) {
    if (velocity == 0 && turn == 0) {
        stop();
        return;
    }
    int16_t pwmRight = velocity + turn;
    int16_t pwmLeft  = velocity - turn;
    int16_t maxMag = max(abs(pwmRight), abs(pwmLeft));
    if (maxMag > MOTOR_MAX_PWM) {
        pwmRight = pwmRight * MOTOR_MAX_PWM / maxMag;
        pwmLeft  = pwmLeft  * MOTOR_MAX_PWM / maxMag;
    }
    _motorRight.run(pwmRight);
    _motorLeft.run(pwmLeft);
    _stopwatch.start();
}

void DriveController::driveDiscreteArcade(uint8_t velocityPWM, uint8_t turnPWM, bool up, bool down, bool right, bool left) {
    int16_t velocity = (up && !down)
                        ? velocityPWM
                        : (down && !up)
                            ? -(int16_t)velocityPWM
                            : 0;
    int16_t turn = (right && !left)
                    ? turnPWM
                    : (left && !right)
                        ? -(int16_t)turnPWM
                        : 0;
    driveDifferential(velocity, turn);
}

void DriveController::driveFor(int16_t velocity, int16_t turn, uint32_t ms) {
    if (ms == 0)
        return;
    _timer.setTimeout(ms);
    _timer.start();
    driveDifferential(velocity, turn);
}

void DriveController::driveDistance(int16_t velocity, double meters) {
    if (meters <= 0)
        return;
    _encoderRight.reset();
    _encoderLeft.reset();
    _targetTicks = metersToTicks(meters, _circumference, _config.encoderSlots);
    driveDifferential(velocity, 0);
}

void DriveController::setMode(const char *mode) {
    if (!mode) 
        return;
    if(strcmp(mode, "auto") == 0) {
        setModeAuto();
        return;
    }
    if (strcmp(mode, "manual") == 0)
        setModeManual();
}

void DriveController::setModeAuto() { _mode = DRIVE_MODE_AUTO; }

void DriveController::setModeManual() { _mode = DRIVE_MODE_MANUAL; }

bool DriveController::isModeAuto() const { return _mode == DRIVE_MODE_AUTO; }

bool DriveController::isModeManual() const { return _mode == DRIVE_MODE_MANUAL; }

bool DriveController::isDriving() const { return _motorRight.getPWM() > 0 || _motorLeft.getPWM() > 0; }

uint32_t DriveController::getDistanceTicks() const { 
    uint32_t a = _encoderRight.getCount();
    uint32_t b = _encoderLeft.getCount();
    return (a >> 1) + (b >> 1) + ((a & 1) + (b & 1) + 1) / 2;
}

double DriveController::getDistanceMeters() const { return ticksToMeters(getDistanceTicks(), _circumference, _config.encoderSlots); }

uint32_t DriveController::getDurationMs() const { return _stopwatch.lap(); }

/** TODO: add remaining meters/ms */
void DriveController::getStatus(JsonObject &target) const {
    bool driving = isDriving();
    target["mode"] = modeToString(_mode);
    target["driving"] = driving;
    if (driving) {
        target["pwmRight"] = _motorRight.getPWM();
        target["pwmLeft"] = _motorLeft.getPWM();;
        target["distance"] = getDistanceMeters();
        target["duration"] = getDurationMs();
    }
}

void DriveController::getConfig(DriveConfig &target) const {
    target.frequency = _config.frequency;
    target.resolution = _config.resolution;
    target.wheelDiameter = _config.wheelDiameter;
    target.encoderSlots = _config.encoderSlots;
    target.motorRightMinPWM = _config.motorRightMinPWM;
    target.motorLeftMinPWM = _config.motorLeftMinPWM;
}

void DriveController::resetConfig() {
    _storage.reset(); 
    updateFrequency(DriveDefaults::frequency);
    updateResolution(DriveDefaults::resolution);
    updateWheelDiameter(DriveDefaults::wheelDiameter);
    updateEncoderSlots(DriveDefaults::encoderSlots);
    updateMotorRightMinPWM(DriveDefaults::motorRightMinPWM);
    updateMotorLeftMinPWM(DriveDefaults::motorLeftMinPWM);
}

void DriveController::updateConfig(DriveConfig &cfg) { 
    updateFrequency(cfg.frequency);
    updateResolution(cfg.resolution);
    updateWheelDiameter(cfg.wheelDiameter);
    updateEncoderSlots(cfg.encoderSlots);
    updateMotorRightMinPWM(cfg.motorRightMinPWM);
    updateMotorLeftMinPWM(cfg.motorLeftMinPWM);
}

/** TODO: apply */
void DriveController::updateFrequency(uint32_t freq) {
    if (_config.frequency == freq)
        return;
    _config.frequency = freq;
    _storage.saveFrequency(freq);
}

/** TODO: apply */
void DriveController::updateResolution(uint8_t res) {
    if (_config.resolution == res)
        return;
    _config.resolution = res;
    _storage.saveResolution(res);
}

void DriveController::updateWheelDiameter(double diameter) {
    if (fabs(_config.wheelDiameter - diameter) <= 0.001f)
        return;
    _circumference = wheelCircumference(diameter, _config.circCorrection);
    _config.wheelDiameter = diameter;
    _storage.saveWheelDiameter(diameter);
}

void DriveController::updateCircumferenceCorrection(double correction) {
    if (_config.circCorrection == correction)
        return;
    _circumference = wheelCircumference(_config.wheelDiameter, correction);
    _config.circCorrection = correction;
    _storage.saveCircCorrection(correction);
}

void DriveController::updateEncoderSlots(uint8_t slots) {
    if (_config.encoderSlots == slots)
        return;
    _config.encoderSlots = slots;
    _storage.saveEncoderSlots(slots);
}

void DriveController::updateMotorRightMinPWM(uint8_t pwm) {
    if (_config.motorRightMinPWM == pwm)
        return;
    _motorRight.setMinPWM(pwm);
    _config.motorRightMinPWM = pwm;
    _storage.saveMotorRightMinPWM(pwm);
}

void DriveController::updateMotorLeftMinPWM(uint8_t pwm) {
    if (_config.motorLeftMinPWM == pwm)
        return;
    _motorLeft.setMinPWM(pwm);
    _config.motorLeftMinPWM = pwm;
    _storage.saveMotorLeftMinPWM(pwm);
}
