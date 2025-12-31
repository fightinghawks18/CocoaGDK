add_executable(net_test main.c)

target_link_libraries(net_test
        PRIVATE cocoa_platform
)

add_test(
        NAME network_test_app
        COMMAND net_test
)