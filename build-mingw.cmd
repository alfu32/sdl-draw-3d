
rm build/win64/voxd31.exe
gcc -o build/win64/voxd31.exe main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -IF:/Development/raylib/src/external -L. -LF:/Development/c99-raylib-vox/lib/raylib-5.0_win64_mingw-w64/lib -IF:/Development/c99-raylib-vox/lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP