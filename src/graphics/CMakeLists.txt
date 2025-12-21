add_library(cocoa_graphics INTERFACE)
target_include_directories(cocoa_graphics INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(cocoa_graphics
        INTERFACE cocoa_math
)
install(DIRECTORY include/ DESTINATION include)