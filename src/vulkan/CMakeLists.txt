find_package(Vulkan REQUIRED)

add_library(cocoa_vulkan STATIC
        src/vulkan_core.c
        src/vulkan_swapchain.c
)

target_include_directories(cocoa_vulkan
        PUBLIC include
)

target_link_libraries(cocoa_vulkan
        PUBLIC Vulkan::Vulkan
        PUBLIC cocoa_core
        PUBLIC cocoa_graphics
        PRIVATE cocoa_tools
)

target_compile_definitions(cocoa_vulkan PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)