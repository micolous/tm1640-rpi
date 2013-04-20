Using with Python
=================

In order to interact with the display, you must have permissions to control the GPIO pins on the Raspberry Pi.  This typically means you need to run as the ``root`` user.

Initialise the display
----------------------

.. highlight:: pycon

In order to connect to the display, you can use the defaults from the standalone program and use GPIO 0 for data, and GPIO 1 for clock.  You can do this with::

	>>> import tm1640
	>>> display = TM1640()

If the library is not installed correctly, this will throw an error::

	Traceback (most recent call last):
	  File "tm1640.py", line 37, in __init__
	    raise ImportError, 'Could not find libtm1640 in PATH'
	ImportError: Could not find libtm1640 in PATH

If this occurs, make sure that :file:`libtm1640.so` is in your dynamic loader's path.  This typically means it should be located at :file:`/usr/lib/libtm1640.so`, and you need to refresh the dynamic loader's cache by executing :command:`ldconfig`.  For more information, see :manpage:`ld.so(8)`.

If you wish to use a different set of display pins, you can set this in the :py:class:`TM1640` constructor as follows::

	>>> display = TM1640(clock_pin=3, data_pin=2)

You must then switch on the display.  By default, the Python library will turn on the display at maximum brightness with the :py:meth:`TM1640.on` method::

	>>> display.on()

You can also set a specific brightness.  To turn on the display at brightness level 3 (about half brightness)::

	>>> display.on(3)

Clear the display
-----------------

If you turn on the display, it may have the contents of whatever was in the TM1640 IC's memory last time it was used.  You can clear the display with::

	>>> display.clear()

Turn off the display
--------------------

In order to turn off the display, and keep the contents of the display in memory, you can issue the off command::

	>>> display.off()

Write to the display
--------------------

You can write up to 16 characters to the display at once.  If you write less than 16 characters, it will leave the cells that you have not specified in the state that they currently are in.

For example::

	>>> display.write('1234567890123456')
	
To print the current date and time to the screen, you could use :py:meth:`datetime.datetime.strftime`::

	>>> import datetime
	>>> display.write(datetime.datetime.now().strftime('%Y-%m-%d  %H%M'))
	
.. note::

	Unlike regular PCs, the Raspberry Pi lacks a backup battery for it's clock, so will reset back to 1970-01-01 00:00 UTC on losing power.  In order to display accurate time, the Pi's clock will need to be synchronised with an external source, for example, an NTP server.

Writing to part of the display at an offset
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can update parts of the screen instead of the entire display at once.  This allows you to push updates partial updates to the screen from different parts of your program, or improve the speed of updates.

In the previous example, you could update the time only in the previous example with::

	>>> display.write(datetime.datetime.now().strftime('%H%M'), offset=12)


Vertically-inverted mode
^^^^^^^^^^^^^^^^^^^^^^^^

If you are viewing the display in a mirror, you can invert the display vertically, using the :py:const:`tm1640.INVERT_MODE_VERTICAL` parameter::

	>>> display.write('1234567890123456', invert_mode=tm1640.INVERT_MODE_VERTICAL)



Class Reference
---------------

.. automodule:: tm1640
	:members:
	:undoc-members:
	:show-inheritance:
