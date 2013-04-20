# Raspberry Pi Driver for TM1640 display IC #

Userspace driver for [Titan Microelectronics TM1640](http://www.titanmec.com/doce/product-detail-180.html) 16 digit 7 segment LED display controller.

[Arduino version](https://code.google.com/p/tm1638-library/) written by Ricardo Batista, [ported to Raspberry Pi](https://bitbucket.org/furyfire/raspi/src/default/hw/tm1640?at=default) by [FuryFire](https://bitbucket.org/furyfire).

Forked off into a seperate project with a bunch of work done on it by [Michael Farrell](http://micolous.id.au).

## License ##

GPLv3.

## Installation ##

You will need to install the [WiringPi library](https://projects.drogon.net/raspberry-pi/wiringpi/) and scons.

You can then build and install the library and Python bindings with:

    # scons install
	# python setup.py install

## Documentation ##

Documentation is provided in the `doc` folder of this project, and can be generated if Sphinx is installed:

	$ cd doc
	$ make html

Alternatively, documentation with the current `master` version of the code is available online at http://tm1640-rpi.rtfd.org/
