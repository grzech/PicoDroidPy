WHEEL_DRIVER_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(WHEEL_DRIVER_MOD_DIR)/wheel.c
SRC_USERMOD += $(WHEEL_DRIVER_MOD_DIR)/wheel_driver.c
SRC_USERMOD += $(WHEEL_DRIVER_MOD_DIR)/wheel_driver_api.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(WHEEL_DRIVER_MOD_DIR)

SRC_USERMOD_C += $(WHEEL_DRIVER_MOD_DIR)/wheel.c
SRC_USERMOD_C += $(WHEEL_DRIVER_MOD_DIR)/wheel_driver.c
SRC_USERMOD_C += $(WHEEL_DRIVER_MOD_DIR)/wheel_driver_api.c