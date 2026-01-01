cmake_minimum_required(VERSION 3.20)

project(cocoa_gdk
        LANGUAGES C CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")

option(BUILD_OPENGL "Allow cocoa_gdk::cocoa_gdk to include opengl, and add opengl tests" OFF)
option(BUILD_VULKAN "Allow cocoa_gdk::cocoa_gdk to include vulkan, and add vulkan tests" OFF)
option(BUILD_SHARED_LIBS "Build project dynamically" OFF)
option(INSTALL_BINARY_DIR "Where binaries are installed" bin)
option(INSTALL_LIBRARY_DIR "Where libraries are installed" lib)

if(BUILD_SHARED_LIBS)
    add_compile_definitions(BUILD_SHARED)
endif()

add_compile_definitions(
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

add_subdirectory("external/VulkanMemoryAllocator")

add_subdirectory("src/core")
add_subdirectory("src/graphics")
add_subdirectory("src/math")
add_subdirectory("src/platform")

# Namespace aliases
add_library(cocoa_gdk::core ALIAS cocoa_core)
add_library(cocoa_gdk::graphics ALIAS cocoa_graphics)
add_library(cocoa_gdk::math ALIAS cocoa_math)
add_library(cocoa_gdk::platform ALIAS cocoa_platform)

include("cmake/content.cmake")

# Main library that bundles everything
add_library(cocoa_gdk INTERFACE)
add_library(cocoa_gdk::cocoa_gdk ALIAS cocoa_gdk)

target_link_libraries(cocoa_gdk INTERFACE
        cocoa_gdk::platform
        cocoa_gdk::core
        cocoa_gdk::graphics
        cocoa_gdk::math
)


# Main tests
add_subdirectory("src/tests/net")

if(BUILD_OPENGL)
    add_subdirectory("src/opengl")
    add_library(cocoa_gdk::opengl ALIAS cocoa_opengl)
    target_link_libraries(cocoa_gdk
            INTERFACE cocoa_gdk::opengl
    )
    add_subdirectory("src/tests/opengl")
endif()

if(BUILD_VULKAN)
    add_subdirectory("src/vulkan")
    add_library(cocoa_gdk::vulkan ALIAS cocoa_vulkan)
    target_link_libraries(cocoa_gdk
            INTERFACE cocoa_gdk::vulkan
    )
    add_subdirectory("src/tests/vulkan")
endif()

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