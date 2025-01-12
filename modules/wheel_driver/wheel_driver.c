// Include MicroPython API.
#include "py/runtime.h"
#include "wheel.h"

#define PWM_FREQ 100

typedef struct _wheel_driver_WheelDriver_obj_t {
    mp_obj_base_t base;
    Wheel_Handle left;
    Wheel_Handle right;
} wheel_driver_WheelDriver_obj_t;

STATIC mp_obj_t wheel_driver_WheelDriver_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_left, ARG_right, ARG_rev_left, ARG_rev_right };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_left,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_right,        MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_rev_left,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_rev_right,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    };
    
    mp_arg_val_t arg_vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, arg_vals);

    wheel_driver_WheelDriver_obj_t *self = m_new_obj(wheel_driver_WheelDriver_obj_t);
    
    self->base.type = type;
    
    self->left = WHEEL_init(arg_vals[ARG_left].u_int, arg_vals[ARG_rev_left].u_int, PWM_FREQ);
    self->right = WHEEL_init(arg_vals[ARG_right].u_int, arg_vals[ARG_rev_right].u_int, PWM_FREQ);
    
    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t wheel_driver_WheelDriver_run(mp_obj_t self_in, const mp_obj_t speed) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    uint8_t s = mp_obj_get_int(speed);
    if (s > Wheel_Speed_MAX) {
        s = Wheel_Speed_MAX;
    }
    WHEEL_set_speed(self->left, s);
    WHEEL_set_speed(self->right, s);

    return MP_ROM_NONE;
}

STATIC void wheel_driver_WheelDriver_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "WheelDriver(left=(Pin%u, Pin%u) right=(Pin%u, Pin%u))",
        WHEEL_get_engine_pin(self->left), WHEEL_get_reverse_pin(self->left),
        WHEEL_get_engine_pin(self->right), WHEEL_get_reverse_pin(self->right));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(wheel_driver_WheelDriver_run_obj, wheel_driver_WheelDriver_run);

STATIC const mp_rom_map_elem_t wheel_driver_WheelDriver_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_set_speed), MP_ROM_PTR(&wheel_driver_WheelDriver_run_obj) },
};

STATIC MP_DEFINE_CONST_DICT(wheel_driver_WheelDriver_locals_dict, wheel_driver_WheelDriver_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    wheel_driver_type_WheelDriver,
    MP_QSTR_WheelDriver,
    MP_TYPE_FLAG_NONE,
    make_new, wheel_driver_WheelDriver_make_new,
    print, wheel_driver_WheelDriver_print,
    locals_dict, &wheel_driver_WheelDriver_locals_dict
    );

STATIC const mp_rom_map_elem_t wheel_driver_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_wheel_driver) },
    { MP_ROM_QSTR(MP_QSTR_WheelDriver),  MP_ROM_PTR(&wheel_driver_type_WheelDriver) },
};

STATIC MP_DEFINE_CONST_DICT(wheel_driver_module_globals, wheel_driver_module_globals_table);

const mp_obj_module_t wheel_driver_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&wheel_driver_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_wheel_driver, wheel_driver_user_cmodule);
