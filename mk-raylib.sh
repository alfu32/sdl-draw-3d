#!/bin/bash

appname="raylib-app"
flags="$(sdl2-config --libs --cflags) -lraylib -lGL -lGLU -lm -lpthread -ldl -lrt -lSDL2_image -lSDL2_ttf"
rm -rf $appname.xe

if [ -f raygui.h ];then
echo "rayguy.h exists"
else
curl "https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h" -O raygui.h
fi

gcc -o $appname.xe $appname.c raygui.h $flags -I/usr/include/SDL2 $*