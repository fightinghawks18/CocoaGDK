cmake_minimum_required(VERSION 3.20)

project(cocoa_engine C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED 17)

add_subdirectory("src/core")
add_subdirectory("src/graphics")
add_subdirectory("src/math")
add_subdirectory("src/opengl")
add_subdirectory("src/platform")
add_subdirectory("src/tools")

include("cmake/content.cmake")

add_executable(cocoa_engine
        src/main.c
)

copy_content_directory("shaders")

target_link_libraries(cocoa_engine
        PRIVATE cocoa_platform
        PRIVATE cocoa_opengl
)

add_compile_definitions(
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

if(WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()