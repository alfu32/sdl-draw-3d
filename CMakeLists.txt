cmake_minimum_required(VERSION 3.10)

# Set your project name
project(YourProjectName)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# SDL2 paths - Adjust these paths as necessary based on your build.ps1 script's directory structure
set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2")
set(SDL2_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf")
set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image")
set(RAYLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/Raylib")

# Find packages
find_package(SDL2 CONFIG REQUIRED PATHS ${SDL2_PATH})
find_package(SDL2_ttf CONFIG REQUIRED PATHS ${SDL2_TTF_PATH})
find_package(SDL2_image CONFIG REQUIRED PATHS ${SDL2_IMAGE_PATH})

# If Raylib find_package is not working, you might need to manually specify the include directory and linked library
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

include_directories(${RAYLIB_PATH}/include)
include_directories(${NUKLEAR_PATH}) # Adjust this path based on your directory structure
include_directories(${RAYGUI_PATH}) # Adjust this path based on your directory structure

# The rest of your CMakeLists.txt configuration goes here...

# Add your source files here
add_executable(${PROJECT_NAME} main.cpp) # Replace main.cpp with the list of your source files

# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_ttf SDL2::SDL2_image raylib)

# Include directories for your project
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_PATH}/include ${SDL2_TTF_PATH}/include ${SDL2_IMAGE_PATH}/include ${RAYLIB_PATH}/include)
