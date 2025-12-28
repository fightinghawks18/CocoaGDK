add_executable(opengl_test main.c)

target_link_libraries(opengl_test
        PRIVATE cocoa_gdk::cocoa_gdk
)

copy_content_directory(opengl_test assets)

add_test(
    NAME app_test
    COMMAND opengl_test
)