# Raspberry Pi Driver for TM1640 display IC #

Written by [FuryFire](https://bitbucket.org/furyfire/raspi).

Forked off into a seperate project with a library by Michael Farrell.

Userspace driver for [TM1640 16 digit 7 segment LED display controller](http://www.dx.com/p/104311).

## Licensing ##

- Original Arduino library is GPLv3
- No license info on RPI version
- Other code in same repo is GPLv2 / GPLv3
- Waiting on FuryFire for clarification, sent message via Bitbucket on 2013-04-12.


## Resources ##

- [Arduino TM1638/1640 library](http://code.google.com/p/tm1638-library/)
- [Low level TM1640 interface notes](http://tronixstuff.wordpress.com/2012/04/09/arduino-and-tm1640-led-display-modules/)
- [Manufacturer's website for TM1640 IC](http://www.titanmec.com/doce/product-detail-180.html)
- [Datasheet for TM1640 IC](http://db.tt/3lrAsvHI) (In Chinese on pages 1 - 10, English version starts on page 11)

## Installation ##

You will need to install the [WiringPi library](https://projects.drogon.net/raspberry-pi/wiringpi/) and scons.

You can then build and install the library and Python bindings with:

    # scons install
	# python setup.py install

## Usage ##

WiringPi has a [GPIO numbering scheme](https://projects.drogon.net/raspberry-pi/wiringpi/pins/) which doesn't match the pins on the Pi directly.

- `VCC` on display -> Pin 2 on Pi `+5v`
- `GND` on display -> Pin 6 on Pi `0v` / `GND`
- `DIN` on display -> Pin 11 on Pi `GPIO 0` (BCM GPIO 17)
- `CLK` on display -> Pin 12 on Pi `GPIO 1` (BCM GPIO 18)

To use this at the command line, run the tool as root:

    # tm1640 on 7
	# tm1640 clear
	# tm1640 write "1234567890123456"

You can also invert the display output in order to allow it to be viewed normally in a mirror:

    # tm1640 iwrite "1234567890123456"

And you can turn off the display with:

    # tm1640 off

When using the display with a library, you can customise which pins to use for data and clock.  This should allow you to use multiple TM1640 units on one Pi, though this is untested.

There are library interfaces in C and Python.

## Use in Python ##

Initialise the display:

    >>> import tm1640
	>>> display = tm1640.TM1640()

You can also use the display on different clock and data pins.  For example, to use pin 3 for clock (`CLK`), and pin 2 for data (`DIN`):

    >>> display = tm1640.TM1640(3, 2)

Then to turn on the display and clear it:

    >>> display.on()
	>>> display.clear()

The Python bindings will default to a brightness of 7 (maximum) by default.  If you pass an argument to the `on()` function you can set to other brightness values between 1 (lowest) and 7:

    >>> display.on(1)

Then to write to the display:

    >>> display.write('1234567890123456')

To write to the display in mirrored mode:

    >>> display.write('1234567890123456', invert_mode=tm1640.INVERT_MODE_VERTICAL)

To write at an offset (for partial screen updates):

    >>> display.write('HELLO', offset=5)

## Customising fonts ##

Fonts are defined in `src/libtm1640/fonts.h` you will need to recompile the project in order to apply changes.

It consist of a 1 dimensional table, defining the LED pattern for that character.

Unsupported charecters are simply set to a space. 

- http://en.wikipedia.org/wiki/Seven-segment_display
- http://www.asciitable.com/
- http://www.uize.com/examples/seven-segment-display.html (Use the value listed under GFEDCBA)