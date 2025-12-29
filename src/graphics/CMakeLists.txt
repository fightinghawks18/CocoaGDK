add_library(cocoa_graphics
        src/image_loader.c
)

target_include_directories(cocoa_graphics
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(cocoa_graphics
        PUBLIC cocoa_math
        PUBLIC cocoa_core
)

target_compile_definitions(cocoa_graphics PRIVATE BUILD_GRAPHICS)