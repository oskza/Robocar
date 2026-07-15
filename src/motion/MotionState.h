#ifndef MOTION_STATE_H
#define MOTION_STATE_H
#include <stdint.h>

enum class MotionState : uint8_t { IDLE, MANUAL, TIMED, DISTANCE, ROTATING };
#endif
