add_library(cocoa_platform STATIC)

target_link_libraries(cocoa_platform
        PUBLIC cocoa_core
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
            PRIVATE src/macos/utils_macos.c
            PRIVATE src/macos/input_macos.m
    )

    target_link_libraries(cocoa_platform
            PRIVATE "-framework Cocoa"
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework QuartzCore"
    )
elseif(WIN32)
    target_sources(cocoa_platform
            PRIVATE src/win32/windowing_win32.c
            PRIVATE src/win32/utils_win32.c
    )
endif()