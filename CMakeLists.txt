cmake_minimum_required(VERSION 3.20)
project(UartTerminal LANGUAGES CXX)

# This generates the compile_commands.json in your build folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set global C++ standards so all targets (main and example) inherit them
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(vendor)
add_subdirectory(source)
