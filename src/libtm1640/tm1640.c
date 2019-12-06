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

const char* HIGH = "1";
const char* LOW = "0";

// THIS STRUCTURE IS NOT PART OF THE API.
typedef struct {
	// Clock pin file descriptor
	FILE* fc;
	// Data pin file descriptor
	FILE* fd;
} _tm1640_display_internal;

#define _DISPLAY_INTERNAL ((_tm1640_display_internal *)(display->_internal))

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


int tm1640_displayWrite(tm1640_display* display, int offset, const char * string, char length, int invertMode) {

	int c=0;
	char buffer[33];
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, string, sizeof(buffer));

	// Translate input to segments
	// Return -EINVAL if input string is too long.  Allowance is made for
	// decimal points.
	// TODO: provide function to allow raw writing of segments
	for (c=0; c<length; c++) {

		if (((buffer[c] == '.') && (offset + c) >= 17) || ((buffer[c] != '.') && (offset + c) >= 16)) {
			return -EINVAL;
		}

		buffer[c] = tm1640_ascii_to_7segment(buffer[c]);

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

		// If possible merge the decimal point with the previous
		// character.  This is only possible if it is not the first
		// character or if the previous character has not already had
		// a decimal point merged.
		if(c!=0 && (0b10000000 & buffer[c]) && !(0b10000000 & buffer[c-1])) {
			buffer[c-1] |= 0b10000000;
			memmove(&buffer[c], (const char *)(&buffer[c+1]), (sizeof(char)*(33-c)));
			memset(&buffer[32], 0, sizeof(char));;
			c--;
			length--;
		}

	}
	tm1640_sendCmd(display, 0x44);
	tm1640_send(display, 0xC0 + offset, buffer, c);
		
	return 0;
}

char tm1640_ascii_to_7segment(char ascii) {
	if (((unsigned char)ascii) < FONT_FIRST_CHAR || 
		((unsigned char)ascii) > FONT_LAST_CHAR) {
		// character than is not in font, skip.
		return 0;
	}

	return DEFAULT_FONT[ascii - FONT_FIRST_CHAR];
}


void tm1640_displayClear(tm1640_display* display)
{
	char buffer[16];
	memset( buffer, 0x00, 16 );
	tm1640_send(display, 0xC0, buffer, 16 );
}

void tm1640_displayOn(tm1640_display* display, char brightness) {
	if (brightness < 1) brightness = 1;
	if (brightness > 7) brightness = 7;
	
	tm1640_sendCmd(display, 0x88 + brightness);
}


void tm1640_displayOff(tm1640_display* display) {
	tm1640_sendCmd(display, 0x80);
}

FILE* _tm1640_open_helper(const char* fileName) {
	FILE* fd = fopen(fileName, "w");
	if (fd == NULL) {
		perror(fileName);
	}
	return fd;
}

int _tm1640_write_and_close(const char* fileName, const char* data, int len) {
	FILE* fd = _tm1640_open_helper(fileName);
	if (fd == NULL) {
		return -1;
	}
	fwrite(data, len, 1, fd);
	fclose(fd);
	return 0;
}

FILE* _tm1640_open_pin(int pin) {
	#define BUF_MAX 30
	char buf[BUF_MAX];
	int len = snprintf(buf, BUF_MAX, "%d", pin);

	// Export the pin
	int err = _tm1640_write_and_close("/sys/class/gpio/export", buf, len);
	if (err != 0) return NULL;

	// Set direction = out
	snprintf(buf, BUF_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	err = _tm1640_write_and_close(buf, "out", 3);

	// Open the FD
	snprintf(buf, BUF_MAX, "/sys/class/gpio/gpio%d/value", pin);
	FILE* fd = _tm1640_open_helper(buf);	
	return fd;
}

void _tm1640_digital_write(FILE* fd, int state) {
	fwrite(state > 0 ? HIGH : LOW, 1, 1, fd);
	fflush(fd);
}

tm1640_display* tm1640_init(int clockPin, int dataPin)
{
	// Setup pins first
	FILE* fc = _tm1640_open_pin(clockPin);
	if (fc == NULL) {
		return NULL;
	}
	
	FILE* fd = _tm1640_open_pin(dataPin);
	if (fd == NULL) {
		fclose(fc);
		return NULL;
	}
	
	// We have open pins, now setup structure
	tm1640_display* display = malloc(sizeof(tm1640_display));
	if (display == NULL) {
		fclose(fc);
		fclose(fd);
		return NULL;
	}
	display->_internal = malloc(sizeof(_tm1640_display_internal));
	if (display->_internal == NULL) {
		free(display);
		fclose(fc);
		fclose(fd);
		return NULL;
	}

	// clear for good measure
	memset(display, 0, sizeof(tm1640_display));	

	_tm1640_digital_write(fc, 1);
	_tm1640_digital_write(fd, 1);
	_DISPLAY_INTERNAL->fc = fc;
	_DISPLAY_INTERNAL->fd = fd;
	return display;
}


void tm1640_destroy(tm1640_display* display) {
	if (display == NULL) {
		return;
	}

	if (display->_internal != NULL) {
		fclose(_DISPLAY_INTERNAL->fc);
		fclose(_DISPLAY_INTERNAL->fd);
		free(display->_internal);
		display->_internal = NULL;
	}

	free(display);
}


void tm1640_sendRaw(tm1640_display* display, char out )
{
	int i;
	for(i = 0; i < 8; i++)
	{
		_tm1640_digital_write(_DISPLAY_INTERNAL->fd, out & (1 << i));
		_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 1);
		usleep(1);
		_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 0);
	}
}

void tm1640_send(tm1640_display* display, char cmd, char * data, int len)
{
	// Issue start command
	_tm1640_digital_write(_DISPLAY_INTERNAL->fd, 0);
	usleep(1);
	_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 0);

	tm1640_sendRaw(display, cmd);
	if(data != NULL)
	{
		int i;
		for(i = 0; i < len; i++)
		{
			tm1640_sendRaw(display, data[i] );
		}
	}
	//Issue stop command
	_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 1);
	usleep(1);
	_tm1640_digital_write(_DISPLAY_INTERNAL->fd, 1);
}


void tm1640_sendCmd(tm1640_display* display, char cmd )
{
	tm1640_send(display, 0x40, 0, 0);
	tm1640_send(display, cmd, 0, 0);

	_tm1640_digital_write(_DISPLAY_INTERNAL->fd, 0);
	_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 0);
	_tm1640_digital_write(_DISPLAY_INTERNAL->fc, 1);
	_tm1640_digital_write(_DISPLAY_INTERNAL->fd, 1);
}
