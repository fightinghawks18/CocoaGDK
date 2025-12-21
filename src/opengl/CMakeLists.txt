find_package(OpenGL REQUIRED)

add_library(cocoa_opengl STATIC
        src/opengl_vao.c
        src/opengl_vbo.c
        src/opengl_ebo.c
        src/opengl_ubo.c
        src/opengl_shader.c
        src/opengl_pipeline.c
        src/opengl_core.c

)
target_include_directories(cocoa_opengl PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(cocoa_opengl
        PUBLIC OpenGL::GL
        PUBLIC cocoa_core
        PUBLIC cocoa_graphics
)

target_compile_definitions(cocoa_opengl
        PRIVATE CCO_BUILDING_OPENGL
)

if(APPLE)
    target_sources(cocoa_opengl
            PRIVATE src/platform/macos/opengl_context_macos.m
            PRIVATE src/platform/macos/opengl_loader_macos.c
    )

    target_link_libraries(cocoa_opengl
            PRIVATE "-framework AppKit"
    )
elseif(WIN32)
    target_sources(cocoa_opengl
            PRIVATE src/platform/win32/opengl_context_win32.c
            PRIVATE src/platform/win32/opengl_loader_win32.c
    )
endif()

install(DIRECTORY include/ DESTINATION include)