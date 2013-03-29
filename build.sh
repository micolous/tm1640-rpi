#!/bin/bash
cc src/main.c  -I./src/ -I/usr/local/include -L/usr/local/lib -lwiringPi -o tm1640

