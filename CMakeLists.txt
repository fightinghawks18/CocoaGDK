cmake_minimum_required(VERSION 3.20)

project(cocoa_gdk C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory("src/core")
add_subdirectory("src/graphics")
add_subdirectory("src/math")
add_subdirectory("src/opengl")
add_subdirectory("src/platform")

# Namespace aliases
add_library(cocoa_gdk::core ALIAS cocoa_core)
add_library(cocoa_gdk::graphics ALIAS cocoa_graphics)
add_library(cocoa_gdk::math ALIAS cocoa_math)
add_library(cocoa_gdk::opengl ALIAS cocoa_opengl)
add_library(cocoa_gdk::platform ALIAS cocoa_platform)

include("cmake/content.cmake")

# Main library that bundles everything
add_library(cocoa_gdk INTERFACE)
add_library(cocoa_gdk::cocoa_gdk ALIAS cocoa_gdk)

target_link_libraries(cocoa_gdk INTERFACE
        cocoa_gdk::platform
        cocoa_gdk::opengl
        cocoa_gdk::core
        cocoa_gdk::graphics
        cocoa_gdk::math
)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(cocoa_move_compiled_commands ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_BINARY_DIR}/../compile_commands.json
    )
endif()

if(WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()