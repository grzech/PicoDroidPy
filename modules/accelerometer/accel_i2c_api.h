#ifndef ACCEL_I2C_API_H_
#define ACCEL_I2C_API_H_

#include "hardware/i2c.h"
#include "hardware/gpio.h"

void ACCEL_Initialize_comm(uint8_t i2c_idx, uint8_t sda_pin, uint8_t scl_pin);
void ACCEL_Write_Register(uint8_t i2c_idx, uint8_t chip_address, uint8_t reg_address, uint8_t reg_value);
uint8_t ACCEL_Read_Register(uint8_t i2c_idx, uint8_t chip_address, uint8_t reg_address);

#endif /* ACCEL_I2C_API_H_ */