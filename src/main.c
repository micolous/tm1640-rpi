#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>

#define DIN_PIN 0
#define SCLK_PIN 1

#include "font.h"

void init( );
void displayWrite(int offset, const char * string);
char ascii_to_7segment(char ascii);
void displayClear( );
void displayOn( int brightness);
void displayOff( );
void send( char cmd, char * data, int len );
void sendRaw( char out );
void sendCmd( char cmd );


/*
 * 
 */

int main( int argc, char** argv )
{
	init( );
	if(argc > 1)
	{
		if(strcmp( argv[1], "on" ) == 0 && argc == 3)
		{
			displayOn(atoi(argv[2]));
		}
		else if(strcmp( argv[1], "off" ) == 0)
		{
			displayOff();

		}
		else if(strcmp( argv[1], "clear" ) == 0)
		{
			displayClear( );
		}
		else if(strcmp( argv[1], "write" ) == 0)
		{
			printf("Writing\n" );
			displayWrite(0,argv[2]);
		}
		else
		{
			printf( "Invalid command\n" );
			return (EXIT_FAILURE);

		}
	}
	else
	{
		printf( "TM1640 display tool\n" );
		printf( "Usage: tm1640 on | off | clear | write\n" );
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS );
}


void displayWrite(int offset, const char * string) {
	//If we can actually print this
	if(offset + strlen(string) <= 16) {
		char buffer[30];
		int c;
		for(c=0;c<strlen(string);c++) {
			buffer[c] = ascii_to_7segment(string[c]);
			buffer[c] = (buffer[c] & 0xc0) |
				// swap top and bottom
				((buffer[c] & 0x01) << 3) |
				((buffer[c] & 0x08) >> 3) |
				// swap lefts
				((buffer[c] & 0x02) << 1) |
				((buffer[c] & 0x04) >> 1) |
				// swap rights
				((buffer[c] & 0x10) << 1) |
				((buffer[c] & 0x20) >> 1);
		}
		sendCmd( 0x44 );
		send(0xC0 + offset,buffer,c);
	} else {
		printf("Can't print %d letters at %d offset",strlen(string),offset);

	}
}

char ascii_to_7segment(char ascii) {
	int s;
	for(s = 0; s < sizeof(font); s++) {
		if(ascii == font[s][0] )
			return font[s][1];
	}
	return 0x00;
}

/**
 * Clear the display 
 * (Writes 0 to every field)
 */
void displayClear( )
{
	char buffer[16];
	memset( buffer, 0x00, 16 );
	send( 0xC0, buffer, 16 );
}

/**
 * Turns on the display and sets the brightness level
 * @param brightness Brightness of the LED's - This can not be set higher than 7.
 */
void displayOn(int brightness) {
	if(brightness > 7) {
		brightness = 7;
	}
	printf("Display: ON - Brightness: %d\n",brightness);
	sendCmd(0x88 + brightness);
}

/**
 * Turns off the display preserving settings
 */
void displayOff() {
	printf("Display: OFF\n");
	sendCmd(0x80);
}


void init( )
{
	if(wiringPiSetup( ) == -1) 
	{
		printf("Error during wiringPiSetup() - Must be run as root");
		exit( EXIT_FAILURE );
	}
	digitalWrite( SCLK_PIN, HIGH );
	digitalWrite( DIN_PIN, HIGH );

	pinMode( SCLK_PIN, OUTPUT );
	pinMode( DIN_PIN, OUTPUT );

}

/**
 * Send a single byte command and flash the clock afterwards (For some odd unknown reason)
 * @param cmd
 */
void sendCmd( char cmd )
{
	send( 0x40, 0 ,0 );
	send( cmd, 0, 0 );

	digitalWrite( DIN_PIN, LOW );
	digitalWrite( SCLK_PIN, LOW );
	digitalWrite( SCLK_PIN, HIGH );
	digitalWrite( DIN_PIN, HIGH );
}

/**
 * Sends a cmd followed by len amount of data. Includes delay from wiringPi.
 * Bitbanging the output pins too fast creates unpredictable results.
 * @param cmd The command
 * @param data Pointer to data that should be appended
 * @param len Lenght of data. If 0 only cmd is sent.
 */
void send( char cmd, char * data, int len )
{
	//Issue start command
	digitalWrite( DIN_PIN, LOW );
	delayMicrosecondsWPi( 1 );
	digitalWrite( SCLK_PIN, LOW );

	sendRaw( cmd );
	if(len)
	{
		int i;
		for(i = 0; i < len; i++)
		{
			sendRaw( data[i] );
		}
	}
	//Issue stop command
	digitalWrite( SCLK_PIN, HIGH );
	delayMicrosecondsWPi( 1 );
	digitalWrite( DIN_PIN, HIGH );
}

/**
 * Shifts out the byte on the port. (The wiringPi implementation is too fast)
 * @param out
 */
void sendRaw( char out )
{
	int i;
	for(i = 0; i < 8; i++)
	{
		digitalWrite( DIN_PIN, out & ( 1 << i ) );
		digitalWrite( SCLK_PIN, HIGH );
		delayMicrosecondsWPi( 1 );
		digitalWrite( SCLK_PIN, LOW );
	}
}
