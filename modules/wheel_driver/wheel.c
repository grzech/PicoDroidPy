#include "wheel.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define PWM_FREQ_KHZ (100)
#define PWM_LEVELS (Wheel_Speed_MAX)

typedef struct {
    uint8_t run_pin;
    uint8_t reverse_pin;
    uint pwm_slice;
    uint pwm_channel;
    Wheel_Speed_t speed;
} Wheel_t;

Wheel_t wheels[WHEELS_NUMBER];
Wheel_Handle configured_wheels = 0;

Wheel_Handle WHEEL_init(uint8_t pin, uint8_t reverse_pin, uint16_t freq_khz)
{
    Wheel_Handle handle = WHEEL_HANDLE_UNDEFINED;

    if(configured_wheels < WHEELS_NUMBER)
    {
        handle = configured_wheels++;
        Wheel_t *wheel = &wheels[handle];
        uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
                
        wheel->run_pin = pin;
        wheel->reverse_pin = reverse_pin;
        wheel->pwm_slice = pwm_gpio_to_slice_num(pin);
        wheel->pwm_channel = pwm_gpio_to_channel(pin);
        wheel->speed = Wheel_Speed_Off;

        gpio_init(reverse_pin);
        gpio_set_dir(reverse_pin, GPIO_OUT);

        gpio_set_function(pin, GPIO_FUNC_PWM);
        pwm_set_clkdiv(wheel->pwm_slice, f_clk_sys/freq_khz*PWM_LEVELS);
        pwm_set_wrap(wheel->pwm_slice, PWM_LEVELS);
        pwm_set_chan_level(wheel->pwm_slice, wheel->pwm_channel, 0);
        pwm_set_enabled(wheel->pwm_slice, true);
    }

    return handle;
}

void WHEEL_set_speed(Wheel_Handle handle, Wheel_Speed_t speed)
{
    if(handle < configured_wheels)
    {
        Wheel_t *wheel = &wheels[handle];
        pwm_set_chan_level(wheel->pwm_slice, wheel->pwm_channel, speed);
        wheel->speed = speed;
    }
}

void WHEEL_set_reverse(Wheel_Handle handle, bool reverse)
{
    if(handle < configured_wheels)
    {
        gpio_put(wheels[handle].reverse_pin, reverse);
    }
}

Wheel_Speed_t WHEEL_get_speed(Wheel_Handle handle) {
    Wheel_Speed_t speed = Wheel_Speed_Off;

    if(handle < configured_wheels)
    {
        speed = wheels[handle].speed;
    }
    return speed;
}

bool WHEEL_get_reverse(Wheel_Handle handle) {
    bool state = false;

    if(handle < configured_wheels)
    {
        state = gpio_get(wheels[handle].reverse_pin);
    }
    return state;
}

uint8_t WHEEL_get_engine_pin(Wheel_Handle handle) {
    uint8_t pin = 0;

    if(handle < configured_wheels)
    {
        pin = wheels[handle].run_pin;
    }
    return pin;
}

uint8_t WHEEL_get_reverse_pin(Wheel_Handle handle) {
    uint8_t pin = 0;

    if(handle < configured_wheels)
    {
        pin = wheels[handle].reverse_pin;
    }
    return pin;
}
