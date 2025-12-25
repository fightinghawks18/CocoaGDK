find_package(Vulkan)

add_library(cocoa_vulkan STATIC
        src/instance.c
        src/swapchain.c
        src/buffer.c
        src/tools/vma.cpp
)
target_include_directories(cocoa_vulkan
        PUBLIC include
)
target_link_libraries(cocoa_vulkan
        PUBLIC Vulkan::Vulkan
        PUBLIC GPUOpen::VulkanMemoryAllocator
        PUBLIC cocoa_gdk::core
        PUBLIC cocoa_gdk::math
        PUBLIC cocoa_gdk::graphics
)

target_compile_definitions(cocoa_vulkan PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)