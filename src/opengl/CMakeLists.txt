find_package(OpenGL REQUIRED)

add_library(cocoa_opengl
        src/opengl_vao.c
        src/opengl_vbo.c
        src/opengl_ebo.c
        src/opengl_ubo.c
        src/opengl_shader.c
        src/opengl_pipeline.c
        src/opengl_core.c
        src/opengl_loader.c
        src/opengl_image.c

)
target_include_directories(cocoa_opengl
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)


target_link_libraries(cocoa_opengl
        PUBLIC OpenGL::GL
        PUBLIC cocoa_core
        PUBLIC cocoa_graphics
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

target_compile_definitions(cocoa_opengl PRIVATE BUILD_OPENGL)

install(
        TARGETS cocoa_opengl
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
)