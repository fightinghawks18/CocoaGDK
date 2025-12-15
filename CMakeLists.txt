cmake_minimum_required(VERSION 3.20)
project(cocoa_engine C)

add_subdirectory("src/core")
add_subdirectory("src/graphics")
add_subdirectory("src/math")
add_subdirectory("src/platform")
add_subdirectory("src/tools")
add_subdirectory("src/vulkan")
add_subdirectory("src/opengl")

include("cmake/content.cmake")

add_executable(cocoa_engine
        src/main.c
)

copy_content_directory("shaders")

target_link_libraries(cocoa_engine
        PRIVATE cocoa_platform
        PRIVATE cocoa_opengl
)