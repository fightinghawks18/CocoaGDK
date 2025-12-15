add_library(cocoa_tools STATIC
        src/dynamic_array.c
        src/resource_manager.c
)

target_link_libraries(cocoa_tools
        PUBLIC cocoa_core
)

target_include_directories(cocoa_tools
        PUBLIC include
)


target_compile_definitions(cocoa_tools PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)