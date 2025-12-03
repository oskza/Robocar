#include "CompassBMM150.h"

CompassBMM150::CompassBMM150() : DFRobot_BMM150_I2C(&Wire, I2C_ADDRESS_4) {}

bool CompassBMM150::init() {
    if(begin())
        return false;
    setOperationMode(BMM150_POWERMODE_NORMAL);
    setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    setRate(BMM150_DATA_RATE_10HZ);
    setMeasurementXYZ();
    return true;
}
