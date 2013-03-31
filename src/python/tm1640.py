#!/usr/bin/env python

__all__ = [
	'TM1640',
	'INVERT_MODE_NONE',
	'INVERT_MODE_VERTICAL',
]

import ctypes
libtm1640 = ctypes.cdll.LoadLibrary('libtm1640.so')

INVERT_MODE_NONE = 0
INVERT_MODE_VERTICAL = 1

class TM1640(object):
	def __init__(self, clock_pin=1, data_pin=0):
		"""
		Initialise the TM1640 display.
		
		:param clock_pin: WiringPi GPIO that has the clock pin plugged into.
		:type clock_pin: int
		
		:param data_pin: WiringPi GPIO that has the data pin plugged into.
		:type data_pin: int
		"""
		self._display = libtm1640.tm1640_init(clock_pin, data_pin)
		if self._display == 0:
			raise Exception, 'Failed to initialise display'
	
	def __del__(self):
		libtm1640.tm1640_destroy(self._display)
	
	def write(self, string, offset=0, invert_mode=INVERT_MODE_NONE):
		"""
		Writes a string to the display.
		
		String must be less than 16 bytes, less any offset bytes.
		
		Throws an exception on error.
		
		:param string: String to write to the display
		:type string: str
		
		:param offset: Where to start writing the string to on the display.
		:type offset: int
		
		:param invert_mode: Whether to invert the display segments.
		
		"""
		result = libtm1640.tm1640_displayWrite(self._display, offset, string, len(string), invert_mode)
		
		if result != 0:
			raise Exception, 'Failed to write to display'
	
	def on(self, brightness=7):
		assert 1 <= brightness <= 7, 'Brightness value must be 1 - 7'
		
		libtm1640.tm1640_displayOn(self._display, brightness)
	
	def off(self):
		libtm1640.tm1640_displayOff(self._display)
	
	def clear(self):
		libtm1640.tm1640_displayClear(self._display)
