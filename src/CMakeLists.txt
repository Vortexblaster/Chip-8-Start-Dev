target_include_directories(${PROJECT_NAME} PRIVATE headers)

set(SOURCES core.c chip-8.c video.c headers/core.h headers/chip-8.h headers/video.h)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE vendored)
