Building the software
=====================

This library requires you have a Raspberry Pi with Linux installed on it.

In order to build the software, you will need to install the following dependencies:

* gcc
* git (if not building from a tarball)
* Python 2.7 (if you wish to use the Python bindings for the library)
* scons
* `WiringPi`_

.. highlight:: console

If you're using Arch Linux ARM, these are all available via ``pacman``::

	# pacman -S scons wiringpi gcc git python2

These dependencies will take some time to download and install to your SD card.

Once installed, you can build and install the C library and command-line interface with::

	# cd /usr/src
	# git clone https://github.com/micolous/tm1640-rpi.git
	# cd tm1640-rpi
	# scons install

Once this has completed, you may then install the Python bindings with::

	# setup.py install

.. _WiringPi: https://projects.drogon.net/raspberry-pi/wiringpi/