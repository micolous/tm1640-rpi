Using with C
============

.. highlight:: c

In order to use the library in your program, you will need to add the following include line::

	#include <tm1640.h>

When you compile your program, you will need to tell the linker to use :file:`libtm1640.so`.  You can typically do this with the ``-l`` option to :program:`gcc`:

.. code-block:: console

	$ gcc -o myprogram myprogram.c -ltm1640

Initialise the display
----------------------

In order to connect to the display, always need to pass the GPIO pins you wish to use.  This is normally GPIO 0 for data, and GPIO 1 for clock::

	tm1640_display* display = tm1640_init(1, 0);

You should then verify that there was no error while initialising the display.  :c:func:`tm1640_init` returns ``NULL`` on error::

	if (display == NULL) {
		fprintf(stderr, "Error initialising display!\n");
		return EXIT_FAILURE;
	}

You must then switch on the display.  You must specify a brightness level between 1 and 7.  To set the display to maximum brightness::

	tm1640_displayOn(display, 7);

Clear the display
-----------------

If you turn on the display, it may have the contents of whatever was in the TM1640 IC's memory last time it was used.  You can clear the display with::

	tm1640_displayClear(display);

Turn off the display
--------------------

In order to turn off the display, and keep the contents of the display in memory, you can issue the off command::

	tm1640_displayOff(display);

Write to the display
--------------------

You can write up to 16 characters to the display at once.  If you write less than 16 characters, it will leave the cells that you have not specified in the state that they currently are in.

For example::

	char* s = "1234567890123456";
	int result = tm1640_displayWrite(display, 0, s, strlen(s), INVERT_MODE_NONE);
	
You should verify that the display was written to correctly::

	if (result != 0) {
		fprintf(stderr, "write error %d\n", result);
		return EXIT_FAILURE;
	}

Writing to part of the display at an offset
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can update parts of the screen instead of the entire display at once.  This allows you to push updates partial updates to the screen from different parts of your program, or improve the speed of updates.

For example, you could write the last 4 characters of the display (offset 12) with::

	char* s = "test";
	int result = tm1640_displayWrite(display, 12, s, strlen(s), INVERT_MODE_NONE);


Vertically-inverted mode
^^^^^^^^^^^^^^^^^^^^^^^^

If you are viewing the display in a mirror, you can invert the display vertically, using the :c:macro:`INVERT_MODE_VERTICAL` parameter::

	char* s = "1234567890123456";
	int result = tm1640_displayWrite(display, 0, s, strlen(s), INVERT_MODE_VERTICAL);

Library Reference
-----------------

.. doxygenfile:: tm1640.h