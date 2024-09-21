#!/bin/bash

# Get the current commit hash
COMMIT_HASH=$(git rev-parse HEAD)
echo "$COMMIT_HASH"

# Get the latest tag
TAG=$(git describe --tags --abbrev=0 2>/dev/null)
echo "$TAG"

# If TAG is empty, use the first argument as the tag
if [ -z "$TAG" ]; then
    TAG="$1"
fi

# Remove old build artifacts
rm -f build/win64/*.exe
rm -f *.7z

# Compile the executable
echo "# Compile the executable"
x86_64-w64-mingw32-gcc -o build/win64/voxd31.exe main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -I./raylib/src/external -L. -L./lib/raylib-5.0_win64_mingw-w64/lib -I./lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP

echo "# Compile the shared library (DLL)"
# Compile the shared library (DLL)
x86_64-w64-mingw32-gcc -shared -o build/win64/voxd31.dll main.c -Wl,--output-def,build/win64/voxd31.def  -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -I. -I./raylib/src/external -L. -L./lib/raylib-5.0_win64_mingw-w64/lib -I./lib/raylib-5.0_win64_mingw-w64/include -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP

# Create a compressed archive with 7z
"C:\Program Files\7-Zip\7z.exe" a "build/voxd31-${TAG}-x86_64-windows" ./build/win64/*
