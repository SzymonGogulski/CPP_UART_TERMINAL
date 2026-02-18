cmake_minimum_required(VERSION 3.20)

project(uart_test LANGUAGES CXX)

# add_subdirectory(vendor)

add_subdirectory(source)
# add_executable(${PROJECT_NAME} source/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

