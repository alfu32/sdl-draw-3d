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
mkdir -p build/x86_64-linux-gnu
echo "zig cc -o build/x86_64-linux-gnu/$appname.xe $appname.c $flags $zig_flags $*"
zig cc -o build/x86_64-linux-gnu/$appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $*
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target aarch64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-linux-musl
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target riscv64-linux-gnu
#zig cc -o $appname.xe $appname.c $flags $zig_flags -I/usr/include/SDL2 $* -target x86_64-windows-msvc

MINGW="$(which x86_64-w64-mingw32-gcc)"
if [ "$MINGW" == "" ] ;then
    sudo apt install mingw-w64
fi

## mkdir -p build/x86_64-windows-gnu
## 
## zig cc  -target x86_64-windows-gnu \
## -I./libs/mesa/include/EGL \
## -I./usr/x86_64-w64-mingw32/include \
## -I./libs/SDL2-2.30.1/x86_64-w64-mingw32/include \
## -I./libs/SDL2-2.30.1/x86_64-w64-mingw32/include/SDL2 \
## -I./libs/Raylib/raylib-5.0_win64_mingw-w64/include \
## -I./libs/SDL2_image-2.8.2/x86_64-w64-mingw32/include \
## -I./libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32/include \
## -L./libs/mesa/lib/x64 \
## -L./libs/opengl \
## -L/usr/x86_64-w64-mingw32/lib \
## -L./libs/SDL2-2.30.1/x86_64-w64-mingw32/lib -L./libs/SDL2-2.30.1/x86_64-w64-mingw32/bin \
## -L./libs/Raylib/raylib-5.0_win64_mingw-w64/lib \
## -L./libs/SDL2_image-2.8.2/x86_64-w64-mingw32/lib -L./libs/SDL2_image-2.8.2/x86_64-w64-mingw32/bin \
## -L./libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32/lib -L./libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32/bin \
## -lwinpthread -lSDL2 -lraylibdll -lopengl32 -lglu32 -lSDL2_image -lSDL2_ttf $zig_flags \
## -o build/x86_64-windows-gnu/$appname.exe $appname.c
## 
## 
## cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll ./build/x86_64-windows-gnu/libwinpthread-1.dll
## cp ./libs/SDL2-2.30.1/x86_64-w64-mingw32/bin/SDL2.dll ./build/x86_64-windows-gnu/
## cp ./libs/Raylib/raylib-5.0_win64_mingw-w64/lib/*.dll ./build/x86_64-windows-gnu/
## cp ./libs/SDL2_image-2.8.2/x86_64-w64-mingw32/bin/*.dll ./build/x86_64-windows-gnu/
## cp ./libs/SDL2_ttf-2.22.0/x86_64-w64-mingw32/bin/*.dll ./build/x86_64-windows-gnu/
## cp ./libs/opengl/*.dll ./build/x86_64-windows-gnu/