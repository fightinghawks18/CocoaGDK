add_executable(vulkan_app main.c)

target_link_libraries(vulkan_app
        PRIVATE cocoa_gdk::platform
        PRIVATE cocoa_gdk::vulkan
)

add_test(
    NAME vulkan_test
    COMMAND vulkan_app
)