// Include MicroPython API.
#include "py/runtime.h"

// Used to get the time in the Timer class accelerometer.
#include "py/mphal.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"

uint8_t data;

typedef struct _accelerometer_Accelerometer_obj_t {
    mp_obj_base_t base;
    i2c_inst_t *i2c;
    mp_int_t addr;
    uint8_t i2c_idx;
    uint8_t scl;
    uint8_t sda;
} accelerometer_Accelerometer_obj_t;

STATIC mp_obj_t accelerometer_Accelerometer_get_chip_id(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    data = 0x00;
    i2c_write_blocking(self->i2c, self->addr, &data, 1, true);
    i2c_read_blocking(self->i2c, self->addr, &data, 1, false);
    return (mp_obj_t) &data;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_get_chip_id_obj, accelerometer_Accelerometer_get_chip_id);

STATIC mp_obj_t accelerometer_Accelerometer_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_i2c, ARG_sdo, ARG_scl, ARG_addr };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_sdo,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_scl,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT,  {.u_int = 0} },
    };
    
    mp_arg_val_t arg_vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, arg_vals);

    accelerometer_Accelerometer_obj_t *self = m_new_obj(accelerometer_Accelerometer_obj_t);
    
    self->base.type = type;
    
    self->i2c_idx = arg_vals[ARG_i2c].u_int;
    self->sda = arg_vals[ARG_sdo].u_int;
    self->scl = arg_vals[ARG_scl].u_int;
    self->addr = arg_vals[ARG_addr].u_int;

    switch (self->i2c_idx) {
        case 0: self->i2c = i2c0; break;
        case 1: self->i2c = i2c1; break;
        default: break;
    }
    
    gpio_set_function(self->sda, GPIO_FUNC_I2C);
    gpio_pull_up(self->sda);
    gpio_set_function(self->scl, GPIO_FUNC_I2C);
    gpio_pull_up(self->scl);
    i2c_init(self->i2c, 200000);

    return MP_OBJ_FROM_PTR(self);
}

STATIC void accelerometer_Accelerometer_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Accelerometer(I2C%u(SDA=GP%u, SCL=GP%u), Address=%u)",
        self->i2c_idx, self->sda, self->scl, self->addr);
}

STATIC const mp_rom_map_elem_t accelerometer_Accelerometer_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get_chip_id), MP_ROM_PTR(&accelerometer_Accelerometer_get_chip_id_obj) },
};
STATIC MP_DEFINE_CONST_DICT(accelerometer_Accelerometer_locals_dict, accelerometer_Accelerometer_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    accelerometer_type_Accelerometer,
    MP_QSTR_Accelerometer,
    MP_TYPE_FLAG_NONE,
    make_new, accelerometer_Accelerometer_make_new,
    print, accelerometer_Accelerometer_print,
    locals_dict, &accelerometer_Accelerometer_locals_dict
    );

STATIC const mp_rom_map_elem_t accelerometer_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_accelerometer) },
    { MP_ROM_QSTR(MP_QSTR_Accelerometer),    MP_ROM_PTR(&accelerometer_type_Accelerometer) },
};
STATIC MP_DEFINE_CONST_DICT(accelerometer_module_globals, accelerometer_module_globals_table);

const mp_obj_module_t accelerometer_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&accelerometer_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_accelerometer, accelerometer_user_cmodule);
