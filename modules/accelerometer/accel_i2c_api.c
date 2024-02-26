#include "accel_i2c_api.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

bool ACCEL_Get_I2c_Inst(uint8_t idx, i2c_inst_t **instance) {
    switch (idx) {
        case 0: *instance = i2c0; break;
        case 1: *instance = i2c1; break;
        default: return false;
    }
    return true;
}

void ACCEL_Initialize_comm(uint8_t i2c_idx, uint8_t sda_pin, uint8_t scl_pin)
{
    i2c_inst_t* i2c = NULL;
    if ( ACCEL_Get_I2c_Inst(i2c_idx, &i2c) ) {
        gpio_set_function(sda_pin, GPIO_FUNC_I2C);
        gpio_pull_up(sda_pin);
        gpio_set_function(scl_pin, GPIO_FUNC_I2C);
        gpio_pull_up(scl_pin);
        i2c_init(i2c, 200000);
    }
}

void ACCEL_Write_Register(uint8_t i2c_idx, uint8_t chip_address, uint8_t reg_address, uint8_t reg_value)
{
    i2c_inst_t* i2c = NULL;
    if ( ACCEL_Get_I2c_Inst(i2c_idx, &i2c) ) {
        const uint8_t data[2] = {reg_address, reg_value};
        i2c_write_blocking(i2c, chip_address, data, 2, false);
    }
}

uint8_t ACCEL_Read_Register(uint8_t i2c_idx, uint8_t chip_address, uint8_t reg_address)
{
    uint8_t data = 0;
    i2c_inst_t* i2c = NULL;
    if ( ACCEL_Get_I2c_Inst(i2c_idx, &i2c) ) {
        i2c_write_blocking(i2c, chip_address, &reg_address, 1, true);
        i2c_read_blocking(i2c, chip_address, &data, 1, false);
    }

    return data;
}
