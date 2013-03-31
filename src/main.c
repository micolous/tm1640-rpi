#include <tm1640.h>

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
		printf( "TM1640 display tool\n" );
		printf( "Usage: tm1640 on | off | clear | write\n" );
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS );
}

