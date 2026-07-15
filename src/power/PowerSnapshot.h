#ifndef POWER_SNAPSHOT_H
#define POWER_SNAPSHOT_H

struct PowerSnapshot {
    bool connected = false;
    bool hasPower = false;
    float volts = 0.0f;
    float milliamps = 0.0f;
    float milliwatts = 0.0f;
};
#endif
