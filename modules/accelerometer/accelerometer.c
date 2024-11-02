// Include MicroPython API.
#include "py/runtime.h"

// Used to get the time in the Timer class accelerometer.
#include "py/mphal.h"

#include "accel_i2c_api.h"

#define RANGE_ADDR 0x22
#define MEAS_X 0x04
#define MEAS_Y 0x06
#define MEAS_Z 0x08

typedef struct _accelerometer_Accelerometer_obj_t {
    mp_obj_base_t base;
    mp_int_t addr;
    uint8_t i2c_idx;
    uint8_t scl;
    uint8_t sda;
} accelerometer_Accelerometer_obj_t;

STATIC mp_obj_t accelerometer_Accelerometer_get_chip_id(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);

    return mp_obj_new_int(ACCEL_Read_Register(self->i2c_idx, self->addr, 0));
}

STATIC mp_obj_t accelerometer_Accelerometer_set_range(mp_obj_t self_in, mp_obj_t range) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    uint8_t rng = mp_obj_get_int(range);
    uint8_t reg_value = ACCEL_Read_Register(self->i2c_idx, self->addr, RANGE_ADDR);
    reg_value &= ~0x3;
    reg_value |= (rng & 0x3);
    ACCEL_Write_Register(self->i2c_idx, self->addr, RANGE_ADDR, reg_value);

    return MP_ROM_NONE;
}

STATIC mp_obj_t accelerometer_Accelerometer_read_x(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    return mp_obj_new_int(ACCEL_Read_Register(self->i2c_idx, self->addr, MEAS_X) >> 2);
}

STATIC mp_obj_t accelerometer_Accelerometer_read_y(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    return mp_obj_new_int(ACCEL_Read_Register(self->i2c_idx, self->addr, MEAS_Y) >> 2);
}

STATIC mp_obj_t accelerometer_Accelerometer_read_z(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    return mp_obj_new_int(ACCEL_Read_Register(self->i2c_idx, self->addr, MEAS_Z) >> 2);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_get_chip_id_obj, accelerometer_Accelerometer_get_chip_id);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(accelerometer_Accelerometer_set_range_obj, accelerometer_Accelerometer_set_range);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_read_x_obj, accelerometer_Accelerometer_read_x);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_read_y_obj, accelerometer_Accelerometer_read_y);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_read_z_obj, accelerometer_Accelerometer_read_z);

STATIC mp_obj_t accelerometer_Accelerometer_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_i2c, ARG_sda, ARG_scl, ARG_addr };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_sda,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_scl,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT,  {.u_int = 0} },
    };
    
    mp_arg_val_t arg_vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, arg_vals);

    accelerometer_Accelerometer_obj_t *self = m_new_obj(accelerometer_Accelerometer_obj_t);
    
    self->base.type = type;
    
    self->i2c_idx = arg_vals[ARG_i2c].u_int;
    self->sda = arg_vals[ARG_sda].u_int;
    self->scl = arg_vals[ARG_scl].u_int;
    self->addr = arg_vals[ARG_addr].u_int;

    ACCEL_Initialize_comm(self->i2c_idx, self->sda, self->scl);

    return MP_OBJ_FROM_PTR(self);
}

STATIC void accelerometer_Accelerometer_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Accelerometer(I2C%u(SDA=GP%u, SCL=GP%u), Address=%u)",
        self->i2c_idx, self->sda, self->scl, self->addr);
}

STATIC const mp_rom_map_elem_t accelerometer_Accelerometer_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get_chip_id), MP_ROM_PTR(&accelerometer_Accelerometer_get_chip_id_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_range), MP_ROM_PTR(&accelerometer_Accelerometer_set_range_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_x), MP_ROM_PTR(&accelerometer_Accelerometer_read_x_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_y), MP_ROM_PTR(&accelerometer_Accelerometer_read_y_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_z), MP_ROM_PTR(&accelerometer_Accelerometer_read_z_obj) },
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
