from machine import I2C
from accelerometer import Accelerometer
from wheel_driver import WheelDriver
from machine import Pin
from utime import sleep

pin = Pin("LED", Pin.OUT)
pin.high()
driver = WheelDriver(16, 17, 18, 19)
print(driver)