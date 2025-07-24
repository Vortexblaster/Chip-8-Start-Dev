set(SOURCES core.c chip-8.c video.c headers/core.h headers/chip-8.h headers/video.h)
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/headers)

target_link_libraries(${PROJECT_NAME} PRIVATE vendored)
