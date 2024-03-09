#!/usr/bin/pwsh

# Define base libraries URLs
$sdl2Url = "https://www.libsdl.org/release/SDL2-devel-2.30.1-VC.zip"
$sdl2TtfUrl = "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip"
$sdl2ImageUrl = "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.2-VC.zip"
$raylibUrl = "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_msvc16.zip"

# Define header-only libraries URLs
$nuklearUrl = "https://github.com/Immediate-Mode-UI/Nuklear/archive/master.zip"
$rayguiUrl = "https://github.com/raysan5/raygui/archive/master.zip"

# Create directories for the libraries
$libsDir = "libs"
$sdl2Dir = Join-Path $libsDir "SDL2"
$sdl2TtfDir = Join-Path $libsDir "SDL2_ttf"
$sdl2ImageDir = Join-Path $libsDir "SDL2_image"
$raylibDir = Join-Path $libsDir "Raylib"
$nuklearDir = Join-Path $libsDir "Nuklear"
$rayguiDir = Join-Path $libsDir "raygui"

New-Item -ItemType Directory -Force -Path $sdl2Dir, $sdl2TtfDir, $sdl2ImageDir, $raylibDir, $nuklearDir, $rayguiDir

# Function to download and extract libraries
Function DownloadAndExtract ($url, $destination) {
    echo "downloading $url to $destination"
    ls
    $fileName = [System.IO.Path]::GetFileName($url)
    $downloadPath = Join-Path $libsDir $fileName
    Invoke-WebRequest $url -OutFile $downloadPath
    ls
    echo "extracting to $destination"
    Expand-Archive $downloadPath -DestinationPath $destination -Force
    Remove-Item $downloadPath
}

# Download and extract the libraries
Write-Host "Downloading and extracting SDL2, SDL2_ttf, SDL2_image, Raylib, Nuklear, and raygui..."
DownloadAndExtract $sdl2Url $sdl2Dir
DownloadAndExtract $sdl2TtfUrl $sdl2TtfDir
DownloadAndExtract $sdl2ImageUrl $sdl2ImageDir
DownloadAndExtract $raylibUrl $raylibDir
DownloadAndExtract $nuklearUrl $nuklearDir
DownloadAndExtract $rayguiUrl $rayguiDir

Write-Host "Libraries downloaded and extracted."

#$buildDir = Join-Path $PSScriptRoot "build"
#New-Item -ItemType Directory -Force -Path $buildDir
#Set-Location $buildDir

# Configure the project with CMake
# Note: Additional flags can be added according to your project's requirements
#cmake -G "Visual Studio 16 2019" -A x64 -S $PSScriptRoot -B $buildDir

# Build the project with CMake
# You can adjust the configuration (Debug, Release, etc.) as needed
#cmake --build $buildDir --config Release

# Optionally, if you want to specify the target to build, you can use:
# cmake --build $buildDir --target YourTargetName --config Release

# After the build
#Write-Host "Build completed. The binaries are located in the $buildDir/Release directory."

# Navigate back to the script's root directory
#Set-Location $PSScriptRoot

