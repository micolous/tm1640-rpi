INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LIBS    = -lwiringPi


tm1640: src/main.c src/font.h
	cc src/main.c -I/usr/local/include -L/usr/local/lib -L. -lwiringPi -o tm1640
