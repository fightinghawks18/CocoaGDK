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