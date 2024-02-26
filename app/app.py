from machine import I2C
from accelerometer import Accelerometer
from machine import Pin
from utime import sleep

pin = Pin("LED", Pin.OUT)
acc = Accelerometer(0, scl=9, sda=8, address=0xA)
print(acc)
print("0x%02X" % acc.get_chip_id())
