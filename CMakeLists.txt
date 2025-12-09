cmake_minimum_required(VERSION 3.20)
project(cocoa_engine C)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(cocoa_engine
        src/main.c
        src/tools/dynamic_array.c
        src/tools/resource_manager.c
        src/opengl/vbo.c
        src/opengl/ebo.c
        src/opengl/ebo.h
)

target_link_libraries(cocoa_engine
        PRIVATE OpenGL::GL
        PRIVATE glfw
        PRIVATE glad::glad
)