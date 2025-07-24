cmake_minimum_required(VERSION 3.24)
project(CHIP-8 C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(vendored)
add_subdirectory(src)
