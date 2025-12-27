add_library(cocoa_core
        src/log.c
)

target_include_directories(cocoa_core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(cocoa_core
        PRIVATE BUILD_CORE
)

install(
        TARGETS cocoa_core
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
)