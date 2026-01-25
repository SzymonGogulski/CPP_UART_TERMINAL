cmake_minimum_required(VERSION 3.20)

project(UartTerminal LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

# Require C++25
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23)

# Optional: helpful warnings (remove if you don't want them)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

