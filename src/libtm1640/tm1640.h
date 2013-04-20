// src/libtm1640/tm1640.h - Main interface code for TM1640
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


#ifndef TM1640_H
#define TM1640_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>

#define DIN_PIN 0
#define SCLK_PIN 1

#define INVERT_MODE_NONE 0
#define INVERT_MODE_VERTICAL 1


typedef struct {
	int clockPin;
	int dataPin;
} tm1640_display;


tm1640_display* tm1640_init(int clockPin, int dataPin);
void tm1640_destroy(tm1640_display* display);

int tm1640_displayWrite(tm1640_display* display, int offset, const char * string, char length, int invertMode);

char tm1640_ascii_to_7segment(char ascii);

void tm1640_displayClear(tm1640_display* display);

void tm1640_displayOn(tm1640_display* display, int brightness);

void tm1640_displayOff(tm1640_display* display);

void tm1640_send(tm1640_display* display, char cmd, char * data, int len );

void tm1640_sendRaw(tm1640_display* display, char out );

void tm1640_sendCmd(tm1640_display* display, char cmd );

#endif
