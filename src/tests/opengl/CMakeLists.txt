add_executable(opengl_test main.c)

target_link_libraries(opengl_test
        PRIVATE cocoa_gdk::cocoa_gdk
)

target_compile_definitions(opengl_test PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

copy_content_directory(opengl_test assets)

add_test(
    NAME opengl_app
    COMMAND opengl_test
)