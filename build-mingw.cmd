
for /F %%G in ('git rev-parse HEAD') do set COMMIT_HASH=%%G
echo %COMMIT_HASH%

for /F %%G in ('git describe --tags --abbrev=0') do set TAG=%%G
echo %TAG%
@echo off

REM Get the current tag
for /F %%G in ('git describe --tags --abbrev=0') do set TAG=%%G

REM Check if TAG is empty
if "%TAG%"=="" (
    set TAG="%1"
)

del build/win64/*.exe
del *.7z

x86_64-w64-mingw32-gcc -o build/win64/voxd31.exe main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -I./raylib/src/external -L. -L./lib/raylib-5.0_win64_mingw-w64/lib -I./lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP
x86_64-w64-mingw32-gcc -shared -o build/win64/voxd31.dll main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -I./raylib/src/external -L. -L./lib/raylib-5.0_win64_mingw-w64/lib -I./lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP

7z a "build/voxd31-%TAG%-x86_64-windows" ./build/win64/*