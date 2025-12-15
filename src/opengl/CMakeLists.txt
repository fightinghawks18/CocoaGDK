find_package(OpenGL REQUIRED)

add_library(cocoa_opengl STATIC
        src/opengl_vao.c

)
target_include_directories(cocoa_opengl
        PUBLIC include
)

target_link_libraries(cocoa_opengl
        PUBLIC OpenGL::GL
        PUBLIC cocoa_core
        PUBLIC cocoa_graphics
        PRIVATE cocoa_tools
)

if(APPLE)
    target_sources(cocoa_opengl
            PRIVATE src/opengl_context_macos.m
    )

    target_link_libraries(cocoa_opengl
            PRIVATE "-framework AppKit"
    )
endif()