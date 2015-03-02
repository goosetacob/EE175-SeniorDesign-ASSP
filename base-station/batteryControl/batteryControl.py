#!/usr/bin/env python
import time
import os
import RPi.GPIO as GPIO
import json
import datetime

GPIO.setmode(GPIO.BCM)

# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
        if ((adcnum > 7) or (adcnum < 0)):
                return -1
        GPIO.output(cspin, True)

        GPIO.output(clockpin, False)  # start clock low
        GPIO.output(cspin, False)     # bring CS low

        commandout = adcnum
        commandout |= 0x18  # start bit + single-ended bit
        commandout <<= 3    # we only need to send 5 bits here
        for i in range(5):
                if (commandout & 0x80):
                        GPIO.output(mosipin, True)
                else:
                        GPIO.output(mosipin, False)
                commandout <<= 1
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)

        adcout = 0
        # read in one empty bit, one null bit and 10 ADC bits
        for i in range(12):
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)
                adcout <<= 1
                if (GPIO.input(misopin)):
                        adcout |= 0x1

        GPIO.output(cspin, True)

        adcout >>= 1       # first bit is 'null' so drop it
        return adcout

# change these as desired - they're the pins connected from the
# SPI port on the ADC to the Cobbler
SPICLK = 18
SPIMISO = 23
SPIMOSI = 24
SPICS = 25

# set up the SPI interface pins
GPIO.setup(SPIMOSI, GPIO.OUT)
GPIO.setup(SPIMISO, GPIO.IN)
GPIO.setup(SPICLK, GPIO.OUT)
GPIO.setup(SPICS, GPIO.OUT)

#EDIT CODE BELOW TO CHANGE FUNCTIONALITY
#read Analog Values from Chip
try:
    #enable DELAY
    DELAY = -1

    #enable DEBUG
    DEBUG = 1

    #battery data file
    batteryFile = open('batteryFile.json', 'w');

	#battery signal connected to adc 0
	battery_adc = 0

	#thresholds to start/stop charging
	up_threshold = 697
	low_threshold = 674

	#pin controling charge
	battery_pin = 15
	GPIO.setup(battery_pin, GPIO.OUT)

	#battery logic
	batterycontrol = False

	#disconnect battery
	GPIO.output(battery_pin, False)

	#wait 300 sec (5 min) for batteries to stabalize
    if DELAY:
        time.sleep(300)


	while(1):
		battery = readadc(battery_adc, SPICLK, SPIMOSI, SPIMISO, SPICS)

		if DEBUG:
			print "battery (10 bit): ", battery, " | ", int(round(battery/10.24)) ,"%"


		if batterycontrol == True:
			if battery >= up_threshold:
				if DEBUG:
					print "   ___stop charging & rest for 10 min"
				batterycontrol = False
				GPIO.output(battery_pin, True)
                if DELAY:
                    time.sleep(600)
			else:
				if DEBUG:
                    print "   continue charging for 30 min"
				if DELAY:
                    time.sleep(1800)

		elif batterycontrol == False:
			if battery <= low_threshold:
				if DEBUG:
					print "   ___start charging"
				batterycontrol = True
				GPIO.output(battery_pin, False)
			else:
				if DEBUG:
					print "   continue discharging"
        batteryData = [ { 'date': datetime.date, 'time': datetime.time, 'batteryPercent': int(round(battery/10.24))} ]
        batteryFile.write(batteryData)


except KeyboardInterrupt:
	print 'Keyboard Interrupt | cleanup'
	GPIO.cleanup()
