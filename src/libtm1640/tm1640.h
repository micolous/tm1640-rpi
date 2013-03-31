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

#include "font.h"

#define INVERT_MODE_NONE 0
#define INVERT_MODE_VERTICAL 1


typedef struct {
	int clockPin;
	int dataPin;
} tm1640_display;


tm1640_display* tm1640_init(int clockPin, int dataPin);

int tm1640_displayWrite(tm1640_display* display, int offset, const char * string, char length, int invertMode);

char tm1640_ascii_to_7segment(char ascii);

void tm1640_displayClear(tm1640_display* display);

void tm1640_displayOn(tm1640_display* display, int brightness);

void tm1640_displayOff(tm1640_display* display);

void tm1640_send(tm1640_display* display, char cmd, char * data, int len );

void tm1640_sendRaw(tm1640_display* display, char out );

void tm1640_sendCmd(tm1640_display* display, char cmd );

#endif
