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

STATIC void turn_vehicle(Wheel_Handle wheel) {
    uint8_t s = WHEEL_get_speed(wheel);
    if (s < Wheel_Speed_Slow) {
        s = 0;
    } else {
        s -= 2;
    }
    WHEEL_set_speed(wheel, s);
}

STATIC mp_obj_t wheel_driver_WheelDriver_turn_left(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    turn_vehicle(self->left);

    return MP_ROM_NONE;
}


STATIC mp_obj_t wheel_driver_WheelDriver_turn_right(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    turn_vehicle(self->right);

    return MP_ROM_NONE;
}

STATIC mp_obj_t wheel_driver_WheelDriver_go_straight(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    uint8_t l = WHEEL_get_speed(self->left);
    uint8_t r = WHEEL_get_speed(self->right);
    if (l < r) {
        WHEEL_set_speed(self->left, r);
    } else {
        WHEEL_set_speed(self->right, l);
    }

    return MP_ROM_NONE;
}

STATIC mp_obj_t wheel_driver_WheelDriver_go_back(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    WHEEL_set_reverse(self->left, true);
    WHEEL_set_reverse(self->right, true);

    return MP_ROM_NONE;
}


STATIC mp_obj_t wheel_driver_WheelDriver_go_forward(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    WHEEL_set_reverse(self->left, false);
    WHEEL_set_reverse(self->right, false);

    return MP_ROM_NONE;
}

STATIC void wheel_driver_WheelDriver_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "WheelDriver(left=(Pin%u, Pin%u) right=(Pin%u, Pin%u))",
        WHEEL_get_engine_pin(self->left), WHEEL_get_reverse_pin(self->left),
        WHEEL_get_engine_pin(self->right), WHEEL_get_reverse_pin(self->right));
}

STATIC mp_obj_t wheel_driver_WheelDriver_rotate_left(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    wheel_driver_WheelDriver_go_straight(self);
    WHEEL_set_reverse(self->left, true);
    WHEEL_set_reverse(self->right, false);

    return MP_ROM_NONE;
}

STATIC mp_obj_t wheel_driver_WheelDriver_rotate_right(mp_obj_t self_in) {
    wheel_driver_WheelDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
    wheel_driver_WheelDriver_go_straight(self);
    WHEEL_set_reverse(self->left, false);
    WHEEL_set_reverse(self->right, true);

    return MP_ROM_NONE;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(wheel_driver_WheelDriver_run_obj, wheel_driver_WheelDriver_run);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_turn_left_obj, wheel_driver_WheelDriver_turn_left);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_turn_right_obj, wheel_driver_WheelDriver_turn_right);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_go_straight_obj, wheel_driver_WheelDriver_go_straight);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_go_back_obj, wheel_driver_WheelDriver_go_back);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_go_forward_obj, wheel_driver_WheelDriver_go_forward);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_rotate_left_obj, wheel_driver_WheelDriver_rotate_left);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wheel_driver_WheelDriver_rotate_right_obj, wheel_driver_WheelDriver_rotate_right);

STATIC const mp_rom_map_elem_t wheel_driver_WheelDriver_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_set_speed), MP_ROM_PTR(&wheel_driver_WheelDriver_run_obj) },
    { MP_ROM_QSTR(MP_QSTR_left), MP_ROM_PTR(&wheel_driver_WheelDriver_turn_left_obj) },
    { MP_ROM_QSTR(MP_QSTR_right), MP_ROM_PTR(&wheel_driver_WheelDriver_turn_right_obj) },
    { MP_ROM_QSTR(MP_QSTR_straight), MP_ROM_PTR(&wheel_driver_WheelDriver_go_straight_obj) },
    { MP_ROM_QSTR(MP_QSTR_backward), MP_ROM_PTR(&wheel_driver_WheelDriver_go_back_obj) },
    { MP_ROM_QSTR(MP_QSTR_forward), MP_ROM_PTR(&wheel_driver_WheelDriver_go_forward_obj) },
    { MP_ROM_QSTR(MP_QSTR_rotate_left), MP_ROM_PTR(&wheel_driver_WheelDriver_rotate_left_obj) },
    { MP_ROM_QSTR(MP_QSTR_rotate_right), MP_ROM_PTR(&wheel_driver_WheelDriver_rotate_right_obj) },
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
