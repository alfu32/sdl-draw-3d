#!/bin/bash

appname="voxd3i-app"
flags="$(sdl2-config --libs --cflags) -lraylib -lGL -lGLU -lm -lpthread -ldl -lrt -lSDL2_image -lSDL2_ttf"
zig_flags="-std=gnu99 -D_GNU_SOURCE -DGL_SILENCE_DEPRECATION=199309L -fno-sanitize=undefined"
rm -rf $appname.xe

if [ -f raygui.h ];then
echo "rayguy.h exists"
else
curl "https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h" -O raygui.h
fi

if [ -f nuklear.h ];then
echo "nuklear.h exists"
else
curl "https://raw.githubusercontent.com/Immediate-Mode-UI/Nuklear/master/nuklear.h" -O nuklear.h
fi

if [ -f raylib-nuklear.h ];then
echo "raylib-nuklear.h exists"
else
curl "https://raw.githubusercontent.com/RobLoach/raylib-nuklear/master/include/raylib-nuklear.h" -O raylib-nuklear.h
fi
zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $*
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target aarch64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-musl
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target riscv64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-windows-msvc