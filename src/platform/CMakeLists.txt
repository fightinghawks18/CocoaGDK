add_library(cocoa_platform STATIC)

target_link_libraries(cocoa_platform
        PUBLIC cocoa_core
)

target_include_directories(cocoa_platform PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(cocoa_platform PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

if(APPLE)
    target_sources(cocoa_platform PRIVATE
            src/cocoa/cocoa_input.m
            src/cocoa/cocoa_window.m
            src/cocoa/cocoa_utils.c
    )

    target_link_libraries(cocoa_platform
            PRIVATE "-framework Cocoa"
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework QuartzCore"
    )
elseif(WIN32)
    target_sources(cocoa_platform PRIVATE
            src/win32/win32_input.c
            src/win32/win32_utils.c
            src/win32/win32_window.c
    )

    target_compile_definitions(cocoa_platform
            PRIVATE WINVER=0x0A00
            PRIVATE _WIN32_WINNT=0x0A00
    )
endif()