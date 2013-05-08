#!/usr/bin/env python
"""
src/python/tm1640.py - Python interface shim for libtm1640.
Copyright 2013 Michael Farrell <http://micolous.id.au/>

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""
__all__ = [
	'TM1640',
	'INVERT_MODE_NONE',
	'INVERT_MODE_VERTICAL',
]

import ctypes, ctypes.util

INVERT_MODE_NONE = 0
"""
Used by :py:meth:`TM1640.write` to indicate that the display output should be sent normally.
"""

INVERT_MODE_VERTICAL = 1
"""
Used by :py:meth:`TM1640.write` to indicate that the display output segments should be flipped vertically, in order to be displayed correctly through a mirror.
"""

class TM1640(object):
	"""
	Interface class for connecting to a TM1640 display IC over the Raspberry Pi's GPIO pins.
	
	"""
	def __init__(self, clock_pin=1, data_pin=0):
		"""
		Initialises	a connection to the TM1640 display.
		
		:param clock_pin: WiringPi GPIO that has the clock pin plugged into.
		:type clock_pin: int
		
		:param data_pin: WiringPi GPIO that has the data pin plugged into.
		:type data_pin: int
		
		:throws ImportError: If ``libtm1640.so`` could not be found by your dynamic linker.  Typically ``libtm1640.so`` should be installed in to ``/usr/lib``.
		
		:throws Exception: If there is a problem accessing BCM GPIO, typically caused by lack of permissions.
		"""
		# Set this attribute to none in case of a library load error, it means the destructor will not be called.
		self._libtm1640 = None
		
		# Find the library name first.
		tm1640_so = ctypes.util.find_library('tm1640')
		
		if tm1640_so == None:
			raise ImportError, 'Could not find libtm1640 in your library search path'

		self._libtm1640 = ctypes.cdll.LoadLibrary(tm1640_so)

		self._display = self._libtm1640.tm1640_init(clock_pin, data_pin)
		if self._display == 0:
			raise Exception, 'Failed to initialise display'
	
	def __del__(self):
		# free() connection structure.
		if self._libtm1640:
			self._libtm1640.tm1640_destroy(self._display)
	
	def write(self, string, offset=0, invert_mode=INVERT_MODE_NONE):
		"""
		Writes a string to the display.
		
		String must be less than 16 bytes, less any offset bytes.
		
		Throws an exception on error.
		
		:param string: String to write to the display
		:type string: str
		
		:param offset: Where to start writing the string to on the display.
		:type offset: int
		
		:param invert_mode: How to invert the display segments.  This must be set to one of :py:const:`INVERT_MODE_NONE` or :py:const:`INVERT_MODE_VERTICAL`.
		
		"""
		result = self._libtm1640.tm1640_displayWrite(self._display, offset, string, len(string), invert_mode)
		
		if result != 0:
			raise Exception, 'Failed to write to display'
	
	def on(self, brightness=7):
		"""
		Turns on the display.
		
		:param brightness: The brightness level to set on the display, between 1 and 7.  1 is dimmest, 7 is brightest.
		:type brighness: int
		
		"""
		assert 1 <= brightness <= 7, 'Brightness value must be 1 - 7'
		
		self._libtm1640.tm1640_displayOn(self._display, brightness)
	
	def off(self):
		"""
		Switches off the display, retaining the contents of the display in the controller's memory.
		
		"""
		self._libtm1640.tm1640_displayOff(self._display)
	
	def clear(self):
		"""
		Clears the contents of the display.
		
		"""
		self._libtm1640.tm1640_displayClear(self._display)
