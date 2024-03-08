#!/usr/bin/pwsh

$appname = "vxdi-app-main"
$flags = "& {sdl2-config --libs --cflags} -lraylib -lGL -lGLU -lm -lpthread -ldl -lrt -lSDL2_image -lSDL2_ttf"
$zig_flags = "-std=gnu99 -D_GNU_SOURCE -DGL_SILENCE_DEPRECATION=199309L -fno-sanitize=undefined"
Remove-Item -Path "build/$appname.exe" -ErrorAction SilentlyContinue

if (Test-Path "external/raygui.h") {
    Write-Output "rayguy.h exists"
} else {
    Invoke-WebRequest "https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h" -OutFile "external/raygui.h"
}

if (Test-Path "external/nuklear.h") {
    Write-Output "nuklear.h exists"
} else {
    Invoke-WebRequest "https://raw.githubusercontent.com/Immediate-Mode-UI/Nuklear/master/nuklear.h" -OutFile "external/nuklear.h"
}

if (Test-Path "external/raylib-nuklear.h") {
    Write-Output "raylib-nuklear.h exists"
} else {
    Invoke-WebRequest "https://raw.githubusercontent.com/RobLoach/raylib-nuklear/master/include/raylib-nuklear.h" -OutFile "external/raylib-nuklear.h"
}

Write-Output "zig cc -o build/$appname.xe $appname.c $flags $zig_flags"
& zig cc -o "build/$appname.xe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args
#& zig cc -o "$appname.xe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args -target "aarch64-linux-gnu"
#& zig cc -o "$appname.xe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args -target "x86_64-linux-gnu"
#& zig cc -o "$appname.xe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args -target "x86_64-linux-musl"
#& zig cc -o "$appname.xe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args -target "riscv64-linux-gnu"
& zig cc -o "$appname.exe" "$appname.c" $flags $zig_flags -I"/usr/include/SDL2" $args -target "x86_64-windows-msvc"
