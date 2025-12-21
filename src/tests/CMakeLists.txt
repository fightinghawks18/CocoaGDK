add_executable(test_app main.c)

target_link_libraries(test_app
        PRIVATE cocoa_gdk::cocoa_gdk
)

add_test(
    NAME app_test
    COMMAND test_app
)