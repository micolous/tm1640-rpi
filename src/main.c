// src/main.c - Test program for interfacing with libtm1640
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

#include <tm1640.h>

int main( int argc, char** argv )
{
	tm1640_display* display = tm1640_init(1, 0);
	if (display == NULL) {
		fprintf(stderr, "%s: display initialisation failed\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	if(argc > 1)
	{
		if(strcmp( argv[1], "on" ) == 0 && argc == 3)
		{
			tm1640_displayOn(display, atoi(argv[2]));
		}
		else if(strcmp( argv[1], "off" ) == 0)
		{
			tm1640_displayOff(display);
		}
		else if(strcmp( argv[1], "clear" ) == 0)
		{
			tm1640_displayClear(display);
		}
		else if(strcmp( argv[1], "write" ) == 0)
		{
			int result = tm1640_displayWrite(display, 0, argv[2], strlen(argv[2]), INVERT_MODE_NONE);
			if (result != 0) {
				fprintf(stderr, "%s: error %d\n", argv[0], result);
				return (EXIT_FAILURE);
			}
		} else if (strcmp(argv[1], "iwrite") == 0) {
			int result = tm1640_displayWrite(display, 0, argv[2], strlen(argv[2]), INVERT_MODE_VERTICAL);
			if (result != 0) {
				fprintf(stderr, "%s: error %d\n", argv[0], result);
				return (EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "Invalid command\n");
			return (EXIT_FAILURE);

		}
	}
	else
	{
		fprintf(stderr, "TM1640 display tool\n" );
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "  tm1640 on <brightness> : Turn on display, brightness 1 (lowest) - 7 (highest)\n");
		fprintf(stderr, "  tm1640 off             : Turn off display, preserving data.\n");
		fprintf(stderr, "  tm1640 clear           : Clear display.\n");
		fprintf(stderr, "  tm1640 write <msg>     : Write message to display, up to 16 chars.\n");		
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS );
}

