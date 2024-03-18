@echo off

set "VERSION=%1"

REM Check if TAG is empty
if "%VERSION%"=="" (
    for /F %%G in ('git describe --tags --abbrev=0') do set "TAG=%%G"
    echo %TAG%
    set "VERSION=%TAG%"
)
REM Check if TAG is empty
if "%VERSION%"=="" (
    for /F %%G in ('git rev-parse HEAD') do set "COMMIT_HASH=%%G"
    echo %COMMIT_HASH%
    set "VERSION=%COMMIT_HASH%"
)

del build\win64\*.exe
del build\*.7z

gcc -o build/win64/voxd31.exe main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -I./raylib/src/external -L. -L./lib/raylib-5.0_win64_mingw-w64/lib -I./lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP

7z a "build\voxd31-%VERSION%-x86_64-windows.7z" .\build\win64/*