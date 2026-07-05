#ifndef POWER_SNAPSHOT_H
#define POWER_SNAPSHOT_H
struct PowerSnapshot {
    bool connected;
    float busVoltage;
    float currentMilliamps;
    float powerMilliwatts;
};
#endif
