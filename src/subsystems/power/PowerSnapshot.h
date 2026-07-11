#ifndef POWER_SNAPSHOT_H
#define POWER_SNAPSHOT_H

struct PowerSnapshot {
    bool connected;
    bool hasPower;
    float volts;
    float milliamps;
    float milliwatts;
};
#endif
