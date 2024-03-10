#!/bin/bash

# Define base libraries URLs
sdl2Url="https://www.libsdl.org/release/SDL2-devel-2.30.1-mingw.zip"
sdl2TtfUrl="https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-mingw.zip"
sdl2ImageUrl="https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.2-mingw.zip"
raylibUrl="https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip"

# Define header-only libraries URLs
nuklearUrl="https://github.com/Immediate-Mode-UI/Nuklear/archive/master.zip"
rayguiUrl="https://github.com/raysan5/raygui/archive/master.zip"

# Create directories for the libraries
libsDir="libs"
sdl2Dir="${libsDir}"
sdl2TtfDir="${libsDir}"
sdl2ImageDir="${libsDir}"
raylibDir="${libsDir}/Raylib"
nuklearDir="${libsDir}/Nuklear"
rayguiDir="${libsDir}/raygui"

mkdir -p "$sdl2Dir" "$sdl2TtfDir" "$sdl2ImageDir" "$raylibDir" "$nuklearDir" "$rayguiDir"

# Function to download and extract libraries
downloadAndExtract() {
    url="$1"
    destination="$2"
    echo "Downloading $url to $destination"
    fileName=$(basename "$url")
    downloadPath="${libsDir}/${fileName}"
    wget "$url" -O "$downloadPath"
    echo "Extracting to $destination"
    unzip -o "$downloadPath" -d "$destination"
    rm "$downloadPath"
}

# Download and extract the libraries
echo "Downloading and extracting SDL2, SDL2_ttf, SDL2_image, Raylib, Nuklear, and raygui..."
downloadAndExtract "$sdl2Url" "$sdl2Dir"
downloadAndExtract "$sdl2TtfUrl" "$sdl2TtfDir"
downloadAndExtract "$sdl2ImageUrl" "$sdl2ImageDir"

downloadAndExtract "$raylibUrl" "$raylibDir"
downloadAndExtract "$nuklearUrl" "$nuklearDir"
downloadAndExtract "$rayguiUrl" "$rayguiDir"

## mv "${libsDir}/SDL2-2.30.1" "${libsDir}/SDL2"
## mv "${libsDir}/SDL2_image-2.8.2" "${libsDir}/SDL2_image"
## mv "${libsDir}/SDL2_ttf-2.22.0" "${libsDir}/SDL2_ttf"

echo "Libraries downloaded and extracted."
