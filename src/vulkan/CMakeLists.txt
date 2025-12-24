find_package(Vulkan)

add_library(cocoa_vulkan STATIC
        src/instance.c
)
target_include_directories(cocoa_vulkan
        PUBLIC include
)
target_link_libraries(cocoa_vulkan
        PUBLIC Vulkan::Vulkan
        PUBLIC cocoa_gdk::core
)