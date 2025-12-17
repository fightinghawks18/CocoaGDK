add_library(cocoa_graphics INTERFACE)
target_include_directories(cocoa_graphics
        INTERFACE include
)
target_link_libraries(cocoa_graphics
        INTERFACE cocoa_math
)