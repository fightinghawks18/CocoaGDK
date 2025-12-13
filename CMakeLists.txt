cmake_minimum_required(VERSION 3.20)
project(cocoa_engine C)

find_package(Vulkan REQUIRED)

include("cmake/content.cmake")

add_executable(cocoa_engine
        src/main.c
        src/tools/dynamic_array.c
        src/tools/resource_manager.c

        src/vulkan/core.c
)

copy_content_directory("shaders")

target_link_libraries(cocoa_engine
        PRIVATE Vulkan::Vulkan
)

target_compile_definitions(cocoa_engine PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

if(APPLE)
    target_sources(cocoa_engine
            PRIVATE src/platform/macos/windowing_macos.m
    )

    target_link_libraries(cocoa_engine
            PRIVATE "-framework Cocoa"
            PRIVATE "-framework CoreFoundation"
    )
endif()