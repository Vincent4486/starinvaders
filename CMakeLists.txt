# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Specify the C++ standard to use
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER gcc)

# Define the project name and enable C language
project(starinvaders LANGUAGES C CXX)

# Find all source files in the "src" directory
file(GLOB SOURCES "src/*.c")

# Find zlib and sdl2
find_package(ZLIB REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# Add an executable target named "starinvaders" from the source files in the "src" directory
add_executable(starinvaders ${SOURCES})
target_link_libraries(starinvaders PRIVATE ZLIB::ZLIB)
target_link_libraries(starinvaders PRIVATE SDL2::SDL2)