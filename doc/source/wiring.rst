Wiring the display
==================

The display has four connections required with the Raspberry Pi:

* ``VIN`` (+5v)
* ``GND``
* ``DIN`` (data in)
* ``CLK`` (data clock)

The ``DIN`` and ``CLK`` require use of GPIO connections on the Pi.  These are all exposed on the ``P1`` connector on the Pi's board.

Connect:

* Pin 2 to ``VIN``
* Pin 6 to ``GND``
* Pin 11 (BCM GPIO 17) to ``DIN``
* Pin 12 (BCM GPIO 18) to ``CLK``

You can also wire this to `other GPIO pins`_ on the Raspberry Pi.  There are a total of 12 GPIO lines exposed.  However, the command-line interface will only work when the display is wired using this pin configuration.  The libraries do not have this restriction.

When the Pi is turned on, nothing will appear on the display until you wake it up and send some data.

.. highlight:: console

You can test this with the command-line interface::

	# tm1640 on 7 ; tm1640 clear
	# tm1640 write "Hello"

You should now see the text ``Hello`` on the display.

If it does not appear, you may not have the connections wired properly.  Make sure you have used Pin 11 for ``DIN`` and Pin 12 for ``CLK``.


.. _other GPIO pins: https://projects.drogon.net/raspberry-pi/wiringpi/pins/
