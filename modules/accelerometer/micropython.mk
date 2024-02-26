ACCELEROMETER_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(ACCELEROMETER_MOD_DIR)/accelerometer.c
SRC_USERMOD += $(ACCELEROMETER_MOD_DIR)/accel_i2c_api.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(ACCELEROMETER_MOD_DIR)

SRC_USERMOD_C += $(ACCELEROMETER_MOD_DIR)/accelerometer.c
SRC_USERMOD_C += $(ACCELEROMETER_MOD_DIR)/accel_i2c_api.c