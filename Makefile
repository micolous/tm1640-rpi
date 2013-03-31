INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

#LDFLAGS = -L/usr/local/lib -L
LIBS    = -lwiringPi

all: tm1640 libtm1640.so

tm1640: src/main.c install_lib
	cc src/main.c -o tm1640 $(CFLAGS) $(LIBS) -ltm1640

tm1640.o: src/libtm1640/tm1640.c src/libtm1640/tm1640.h src/libtm1640/font.h
	cc -c -Wall -Werror -fpic -o tm1640.o src/libtm1640/tm1640.c $(CFLAGS) $(LIBS)

libtm1640.so: tm1640.o
	cc -shared -o libtm1640.so tm1640.o $(CFLAGS) $(LIBS)
	
install_lib: libtm1640.so
	install -o0 -g0 libtm1640.so /usr/local/lib/
	ldconfig

install: tm1640
	install -o0 -g0 tm1640 /usr/local/bin/
	
clean:
	rm -f libtm1640.so tm1640 tm1640.o
