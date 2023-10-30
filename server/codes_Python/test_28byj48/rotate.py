#!/usr/bin/env python
import RPi.GPIO as GPIO

# import the library
from RpiMotorLib import RpiMotorLib

GpioPins = [1, 7, 8, 25]

# Declare an named instance of class pass a name and motor type
mymotortest = RpiMotorLib.BYJMotor("MyMotorOne", "28BYJ")

# call the function , pass the parameters
mymotortest.motor_run(GpioPins , 0.002, 1/0, False, False, "full", .05)