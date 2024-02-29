#!/bin/bash

appname="raylib-app"
flags="$(sdl2-config --libs --cflags) -lraylib -lGL -lGLU -lm -lpthread -ldl -lrt -lSDL2_image -lSDL2_ttf"
rm -rf $appname.xe

gcc -o $appname.xe $appname.c $flags -I/usr/include/SDL2 $*