Using with shell scripts
========================

The command-line program ``tm1640`` will let you drive a display from the command-line.

However, it has a number of limitations:

* You cannot write to the screen with offsets, so you have to update the entire screen at once.
* You can only drive one display, using GPIO 18 for clock, and GPIO 17 for data.
* Because of the overheads involved in creating a new process, update speeds will be slower than using the library.

The purpose of supplying a command-line interface is to facilitate testing and rapid prototyping.  It is strongly recommended you write any applications to use the library instead.

Because of restrictions on use of GPIOs for users, you must always run the ``tm1640`` program as the ``root`` user.

.. highlight:: console

Initialise the display
----------------------

In order to turn on the display, you must set it's brightness to a value between 1 (dimmest) and 7 (brightest).

For example, to turn on the display at maximum brightness::

	# tm1640 on 7

Clear the display
-----------------

If you turn on the display, it may have the contents of whatever was in the TM1640 IC's memory last time it was used.  You can clear the display with::

	# tm1640 clear

Turn off the display
--------------------

In order to turn off the display, and keep the contents of the display in memory, you can issue the off command::

	# tm1640 off

Write to the display
--------------------

You can write up to 16 characters to the display at once.  If you write less than 16 characters, it will leave the cells that you have not specified in the state that they currently are in.

For example::

	# tm1640 write "1234567890123456"
	
To print the current date and time to the screen, you can insert the output of the ``date`` command::

	# tm1640 write "`date +%Y-%m-%d  %H%M`"
	
.. note::

	Unlike regular PCs, the Raspberry Pi lacks a backup battery for it's clock, so will reset back to 1970-01-01 00:00 UTC on losing power.  In order to display accurate time, the Pi's clock will need to be synchronised with an external source, for example, an NTP server.

Vertically-inverted mode
^^^^^^^^^^^^^^^^^^^^^^^^

If you are viewing the display in a mirror, you can invert the display vertically.

For example::

	# tm1640 iwrite "1234567890123456"
