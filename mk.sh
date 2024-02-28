#!/bin/bash

appname="minecraft-edit-cubes"
flags="$(sdl2-config --libs --cflags) -lGL -lGLU -lm -lSDL2_image -lSDL2_ttf"
rm -rf $appname.xe

gcc -o $appname.xe $appname.c $flags -I/usr/include/SDL2 $*