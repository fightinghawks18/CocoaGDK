find_package(Vulkan REQUIRED)

add_library(cocoa_platform STATIC)

target_link_libraries(cocoa_platform
        PUBLIC cocoa_core
        PRIVATE Vulkan::Vulkan
)

target_include_directories(cocoa_platform
        PUBLIC include
)

target_compile_definitions(cocoa_platform PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

if(APPLE)
    target_sources(cocoa_platform
            PRIVATE src/macos/windowing_macos.m
    )

    target_link_libraries(cocoa_platform
            PRIVATE "-framework Cocoa"
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework QuartzCore"
    )
endif()