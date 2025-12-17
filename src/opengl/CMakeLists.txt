find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(cocoa_opengl STATIC
        src/gl/opengl_vao.c
        src/gl/opengl_vbo.c
        src/gl/opengl_ebo.c
        src/gl/opengl_ubo.c
        src/gl/opengl_shader.c
        src/gl/opengl_pipeline.c
        src/gl/opengl_core.c

)
target_include_directories(cocoa_opengl
        PUBLIC include
)

target_link_libraries(cocoa_opengl
        PUBLIC OpenGL::GL
        PUBLIC glad::glad
        PUBLIC cocoa_core
        PUBLIC cocoa_graphics
        PRIVATE cocoa_tools
)

if(APPLE)
    target_sources(cocoa_opengl
            PRIVATE src/gl/platform/macos/opengl_context_macos.m
            PRIVATE src/gl/platform/macos/opengl_loader_macos.c
    )

    target_link_libraries(cocoa_opengl
            PRIVATE "-framework AppKit"
    )
elseif(WIN32)
    target_sources(cocoa_opengl
            PRIVATE src/gl/platform/win32/opengl_context_win32.c
            PRIVATE src/gl/platform/win32/opengl_loader_win32.c
    )
endif()