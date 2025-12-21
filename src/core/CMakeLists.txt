add_library(cocoa_core INTERFACE)
target_include_directories(cocoa_core INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
install(DIRECTORY include/ DESTINATION include)