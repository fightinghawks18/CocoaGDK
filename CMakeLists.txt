cmake_minimum_required(VERSION 3.20)
project(everest_engine C)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(everest_engine
    src/main.c
)

target_link_libraries(everest_engine
    PRIVATE OpenGL::GL
    PRIVATE glfw
    PRIVATE glad::glad
)