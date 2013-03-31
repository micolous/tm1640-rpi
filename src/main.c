#include "libtm1640/tm1640.h"

int main( int argc, char** argv )
{
	tm1640_display* display = tm1640_init(1, 0);
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
			tm1640_displayWrite(display, 0, argv[2], strlen(argv[2]), INVERT_MODE_NONE);
		} else if (strcmp(argv[1], "iwrite") == 0) {
			tm1640_displayWrite(display, 0, argv[2], strlen(argv[2]), INVERT_MODE_VERTICAL);
		} else
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

