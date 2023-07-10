# Add custom finds
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/third-party")

# Find Protobuf
set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
find_package(Protobuf 3.12.4)
if(NOT Protobuf_FOUND)
    # Fallback to older Protobuf version (due to Ubuntu 20.04)
    find_package(Protobuf 3.6.1 REQUIRED)
    # Override 'protobuf_generate' with more recent version as 'PLUGIN' keyword is not yet supported
    include("${PROJECT_SOURCE_DIR}/cmake/third-party/protobuf_generate.cmake")
endif()

# Find gRPC
find_package(gRPC REQUIRED)

# Create library
file(GLOB_RECURSE PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto)
add_library(${PROJECT_NAME}-protocol ${PROTO_FILES})
target_include_directories (${PROJECT_NAME}-protocol PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}-protocol PUBLIC gRPC::grpc++ ${Protobuf_LIBRARIES})
protobuf_generate(TARGET ${PROJECT_NAME}-protocol LANGUAGE cpp)
get_target_property(gRPC_CPP_PLUGIN gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ${PROJECT_NAME}-protocol LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${gRPC_CPP_PLUGIN}")

# Install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}-protocol EXPORT ${PROJECT_NAME})
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/blickfeld
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING 
    PATTERN "*.pb.h"
)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/blickfeld
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING 
    PATTERN "*.proto"
)