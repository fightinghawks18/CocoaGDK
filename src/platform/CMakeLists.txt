add_library(cocoa_platform)

target_link_libraries(cocoa_platform
        PUBLIC cocoa_core
        PUBLIC cocoa_math
)

target_include_directories(cocoa_platform
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_compile_definitions(cocoa_platform PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        PRIVATE BUILD_PLATFORM
)

if(APPLE)
    target_sources(cocoa_platform PRIVATE
            src/cocoa/cocoa_input.m
            src/cocoa/cocoa_window.m
            src/cocoa/cocoa_utils.c
            src/cocoa/cocoa_audio.m
            src/cocoa/cocoa_platform.m
    )

    target_link_libraries(cocoa_platform
            PRIVATE "-framework Cocoa"
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework QuartzCore"
            PRIVATE "-framework CoreAudio"
            PRIVATE "-framework AudioToolbox"
    )
elseif(WIN32)
    target_sources(cocoa_platform PRIVATE
            src/win32/win32_input.c
            src/win32/win32_utils.c
            src/win32/win32_window.c
            src/win32/win32_audio.c
            src/win32/win32_platform.c
    )

    target_compile_definitions(cocoa_platform
            PRIVATE WINVER=0x0A00
            PRIVATE _WIN32_WINNT=0x0A00
    )

    target_link_libraries(cocoa_platform
            PRIVATE xaudio2.lib
    )
endif()

install(
        TARGETS cocoa_platform
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
)