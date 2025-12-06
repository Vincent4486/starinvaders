cmake_minimum_required(VERSION 3.10)
project(SDL2_Example LANGUAGES C)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Add your executable target
file(GLOB SOURCES "src/*.c")
add_executable(star-invaders ${SOURCES})

# Link the SDL2 libraries to your executable
target_link_libraries(star-invaders PRIVATE SDL2::SDL2)