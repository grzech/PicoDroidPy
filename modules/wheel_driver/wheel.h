#ifndef __WHEEL_H__
#define __WHEEL_H__

#include "pico/stdlib.h"

#define WHEELS_NUMBER (2)
#define WHEEL_HANDLE_UNDEFINED (-1)

typedef int8_t Wheel_Handle;

typedef enum {
    Wheel_Speed_Off = 0,
    Wheel_Speed_Very_Slow = 2,
    Wheel_Speed_Slow = 3,
    Wheel_Speed_Medium = 4,
    Wheel_Speed_Fast = 5,
    Wheel_Speed_Very_Fast = 6,
    Wheel_Speed_MAX = 11
} Wheel_Speed_t;

Wheel_Handle WHEEL_init(uint8_t pin, uint8_t reverse_pin, uint16_t freq_khz);
void WHEEL_set_speed(Wheel_Handle handle, Wheel_Speed_t speed);
void WHEEL_set_reverse(Wheel_Handle handle, bool reverse);
Wheel_Speed_t WHEEL_get_speed(Wheel_Handle handle);
bool WHEEL_get_reverse(Wheel_Handle handle);
uint8_t WHEEL_get_engine_pin(Wheel_Handle handle);
uint8_t WHEEL_get_reverse_pin(Wheel_Handle handle);

#endif /*__WHEEL_H__*/
