Building the software
=====================

This library requires you have a Raspberry Pi with Linux installed on it.

In order to build the software, you will need to install the following dependencies:

* gcc
* CMake
* Python 2.7 or 3.5+ (optional, for Python bindings)
* git (if not building from a tarball)

.. highlight:: console

If you're using Raspbian::

	$ sudo apt install cmake build-essential

If you're using Arch Linux ARM, these are all available via ``pacman``::

	# pacman -S cmake gcc git python

These dependencies will take some time to download and install to your SD card.

Once installed, you can build and install the C library and command-line interface with::

	# cd /usr/src
	# git clone https://github.com/micolous/tm1640-rpi.git
	# cd tm1640-rpi
	# cmake .
	# make
	# make install

Once this has completed, you may then install the Python bindings with::

	# setup.py install
