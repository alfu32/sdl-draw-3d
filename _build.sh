#!/bin/bash

appname="vxdi-app-main"
flags="$(sdl2-config --libs --cflags) -lraylib -lGL -lGLU -lm -lpthread -ldl -lrt -lSDL2_image -lSDL2_ttf"
zig_flags="-std=gnu99 -D_GNU_SOURCE -DGL_SILENCE_DEPRECATION=199309L -fno-sanitize=undefined"
rm -rf build/$appname.xe

if [ -f external/raygui.h ];then
echo "rayguy.h exists"
else
curl "https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h" -O external/raygui.h
fi

if [ -f external/nuklear.h ];then
echo "nuklear.h exists"
else
curl "https://raw.githubusercontent.com/Immediate-Mode-UI/Nuklear/master/nuklear.h" -O external/nuklear.h
fi

if [ -f external/raylib-nuklear.h ];then
echo "raylib-nuklear.h exists"
else
curl "https://raw.githubusercontent.com/RobLoach/raylib-nuklear/master/include/raylib-nuklear.h" -O external/raylib-nuklear.h
fi
echo "zig cc -o build/$appname.xe $appname.c $flags $zig_flags $*"
zig cc -o build/$appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $*
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target aarch64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-musl
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target riscv64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-windows-msvc


zig cc -o build/$appname.exe $appname.c $flags $zig_flags -I/usr/include/SDL2 -target x86_64-windows-gnu $*