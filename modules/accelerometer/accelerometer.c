// Include MicroPython API.
#include "py/runtime.h"

// Used to get the time in the Timer class accelerometer.
#include "py/mphal.h"

#include "extmod/machine_i2c.h"

typedef struct _accelerometer_Accelerometer_obj_t {
    mp_obj_base_t base;
    mp_machine_i2c_p_t i2c;
    mp_int_t addr;
    mp_machine_i2c_buf_t buf;
    uint8_t data[2];
} accelerometer_Accelerometer_obj_t;

STATIC mp_obj_t accelerometer_Accelerometer_get_chip_id(mp_obj_t self_in) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    self->buf.len = 1;
    self->data[0] = 0x00;
    self->i2c.transfer((mp_obj_t)&self->i2c, self->addr, 1, &self->buf, 0);
    self->i2c.transfer((mp_obj_t)&self->i2c, self->addr, 1, &self->buf, MP_MACHINE_I2C_FLAG_READ);
    
    return (mp_obj_t) &self->buf;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(accelerometer_Accelerometer_get_chip_id_obj, accelerometer_Accelerometer_get_chip_id);

STATIC mp_obj_t accelerometer_Accelerometer_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_i2c, ARG_addr };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT,  {.u_int = 0} },
    };
    
    mp_arg_val_t arg_vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, arg_vals);

    accelerometer_Accelerometer_obj_t *self = m_new_obj(accelerometer_Accelerometer_obj_t);
    self->base.type = type;
    self->i2c = *(mp_machine_i2c_p_t*) arg_vals[ARG_i2c].u_obj;
    self->addr = arg_vals[ARG_addr].u_int;
    self->buf.buf = self->data;

    return MP_OBJ_FROM_PTR(self);
}

STATIC void accelerometer_Accelerometer_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    accelerometer_Accelerometer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Accelerometer(Address=%u)", self->addr);
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
