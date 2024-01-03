from machine import I2C
from accelerometer import Accelerometer
from machine import Pin
from utime import sleep

pin = Pin("LED", Pin.OUT)
i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=200_000)
print(i2c)
acc = Accelerometer(i2c, 0x0A)
print(acc)
sleep(2)
#i2c.writeto(0x0A, b'\x00')
#chip_id = i2c.readfrom(0x0A, 1)
print(acc.get_chip_id())
print("Chip ID: ", acc.buf)

