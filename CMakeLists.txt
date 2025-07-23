cmake_minimum_required(VERSION 3.10)
project(CHIP-8 LANGUAGES CXX)

find_package(SDL3 REQUIRED)

add_executable(CHIP-8 chip-8.c)
add_executable(CHIP-8 video.c)


target_link_libraries(CHIP-8 PRIVATE SDL3::SDL3)

target_include_directories(CHIP-8 PRIVATE ${SDL3_INCLUDE_DIRS})
