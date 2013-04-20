// src/libtm1640/tm1640.c - Main interface code for TM1640
// Copyright 2013 FuryFire
// Copyright 2013 Michael Farrell <http://micolous.id.au/>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "tm1640.h"
#include "font.h"

/**
 * Flips 7-segment characters vertical, for display in a mirror.
 *
 */
char tm1640_invertVertical(char input) {
	return (input & 0xC0) |
		// swap top and bottom
		((input & 0x01) << 3) |
		((input & 0x08) >> 3) |
		// swap lefts
		((input & 0x02) << 1) |
		((input & 0x04) >> 1) |
		// swap rights
		((input & 0x10) << 1) |
		((input & 0x20) >> 1);
}


/**
 * displayWrite
 *  offset: offset on the display to start writing from
 *  string: string to write to the display
 *  length: length of the string to write to the display
 *  invertMode: invert mode to apply to text written to the display
 *
 * Returns -EINVAL if:
 *  - invertMode is invalid
 *  - offset + length is > 16 (size of the display)
 *
 * Returns 0 on success.
 */
int tm1640_displayWrite(tm1640_display* display, int offset, const char * string, char length, int invertMode) {
	//If we can actually print this
	if(offset + length <= 16) {
		char buffer[30];
		memset(buffer, 0, sizeof(buffer));
		
		int c;
		for (c=0; c<length; c++) {
			buffer[c] = tm1640_ascii_to_7segment(string[c]);
			
			switch (invertMode) {
				case INVERT_MODE_NONE:
					// do nothing
					break;
					
				case INVERT_MODE_VERTICAL:
					buffer[c] = tm1640_invertVertical(buffer[c]);
					break;
				
				default:
					return -EINVAL;
			}
		}
		tm1640_sendCmd(display, 0x44 );
		tm1640_send(display, 0xC0 + offset, buffer, c);
		
		return 0;
	} else {
		return -EINVAL;
	}
}


/**
 * Converts an ASCII character into 7 segment binary form for display.
 *
 * Returns 0 (no segments) if the character has no known translation.
 */
char tm1640_ascii_to_7segment(char ascii) {
	if (ascii < FONT_FIRST_CHAR || ascii > FONT_LAST_CHAR) {
		// character than is not in font, skip.
		return 0;
	}
	
	return DEFAULT_FONT[ascii - FONT_FIRST_CHAR];
}

/**
 * Clear the display 
 * (Writes 0 to every field)
 */
void tm1640_displayClear(tm1640_display* display)
{
	char buffer[16];
	memset( buffer, 0x00, 16 );
	tm1640_send(display, 0xC0, buffer, 16 );
}

/**
 * Turns on the display and sets the brightness level
 * @param brightness Brightness of the LED's - This can not be set higher than 7.
 */
void tm1640_displayOn(tm1640_display* display, int brightness) {
	if(brightness > 7) {
		brightness = 7;
	}
	//printf("Display: ON - Brightness: %d\n",brightness);
	tm1640_sendCmd(display, 0x88 + brightness);
}

/**
 * Turns off the display preserving settings
 */
void tm1640_displayOff(tm1640_display* display) {
	//printf("Display: OFF\n");
	tm1640_sendCmd(display, 0x80);
}


/**
 * Initialises the display.
 *
 * Returns pointer to tm1640_display struct on success.
 * Returns NULL if wiringPiSetup() fails (permission error).
 */
tm1640_display* tm1640_init(int clockPin, int dataPin)
{
	if(wiringPiSetup( ) == -1) 
	{
		return NULL;
	}
	digitalWrite(clockPin, HIGH );
	digitalWrite(dataPin, HIGH );

	pinMode(clockPin, OUTPUT );
	pinMode(dataPin, OUTPUT );
	
	tm1640_display* display = malloc(sizeof(tm1640_display));
	
	// clear for good measure
	memset(display, 0, sizeof(tm1640_display));
	
	display->clockPin = clockPin;
	display->dataPin = dataPin;

	return display;

}

/**
 * Destroy the structure associated with the connection to the TM1640.
 */
void tm1640_destroy(tm1640_display* display) {
	free(display);
}


/**
 * Send a single byte command and flash the clock afterwards (For some odd unknown reason)
 * @param cmd
 */
void tm1640_sendCmd(tm1640_display* display, char cmd )
{
	tm1640_send(display, 0x40, 0 ,0 );
	tm1640_send(display, cmd, 0, 0 );

	digitalWrite(display->dataPin, LOW );
	digitalWrite( display->clockPin, LOW );
	digitalWrite( display->clockPin, HIGH );
	digitalWrite( display->dataPin, HIGH );
}

/**
 * Sends a cmd followed by len amount of data. Includes delay from wiringPi.
 * Bitbanging the output pins too fast creates unpredictable results.
 * @param display TM1640 display structure to use for this operation.
 * @param cmd The command
 * @param data Pointer to data that should be appended
 * @param len Length of data. If 0 only cmd is sent.
 
 */
void tm1640_send(tm1640_display* display, char cmd, char * data, int len )
{
	//Issue start command
	digitalWrite( display->dataPin, LOW );
	delayMicroseconds( 1 );
	digitalWrite( display->clockPin, LOW );

	tm1640_sendRaw(display, cmd);
	if(len)
	{
		int i;
		for(i = 0; i < len; i++)
		{
			tm1640_sendRaw(display, data[i] );
		}
	}
	//Issue stop command
	digitalWrite( display->clockPin, HIGH );
	delayMicroseconds( 1 );
	digitalWrite( display->dataPin, HIGH );
}

/**
 * Shifts out the byte on the port. (The wiringPi implementation is too fast)
 * @param display TM1640 display structure to user this for this operation.
 * @param out
 */
void tm1640_sendRaw(tm1640_display* display, char out )
{
	int i;
	for(i = 0; i < 8; i++)
	{
		digitalWrite( display->dataPin, out & ( 1 << i ) );
		digitalWrite( display->clockPin, HIGH );
		delayMicroseconds( 1 );
		digitalWrite( display->clockPin, LOW );
	}
}
