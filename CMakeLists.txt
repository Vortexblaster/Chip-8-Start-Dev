cmake_minimum_required(VERSION 3.24)
project(CHIP-8 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_subdirectory(vendored)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
