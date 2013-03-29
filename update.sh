#!/bin/sh
./tm1640 on 15
./tm1640 clear
while [ 1 ]; do
	#./tm1640 clear
	./tm1640 write "`date '+%^b %d  %H %M %S'`"
	sleep .5s
done

