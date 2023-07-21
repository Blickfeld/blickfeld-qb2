# Load certificate in grpc definitions file
file(READ ${BF_TLS_DEVICE_CA_PATH} BF_TLS_DEVICE_CA_CONTENT)
string(REPLACE "\n" "\\n" BF_TLS_DEVICE_CA_CONTENT ${BF_TLS_DEVICE_CA_CONTENT})
configure_file(include/blickfeld/base/grpc_defs.h.in include/blickfeld/base/grpc_defs.h)

# Add library
add_library(${PROJECT_NAME}-cpp INTERFACE)
target_include_directories(${PROJECT_NAME}-cpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}-cpp EXPORT ${PROJECT_NAME})
install(
    DIRECTORY include/blickfeld/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blickfeld
    FILES_MATCHING PATTERN "*.h"
)
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/blickfeld/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blickfeld
    FILES_MATCHING PATTERN "*.h"
)

# Simple examples
if (BF_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()