# Find Protobuf, 3.12 is the first to compile proto3 optional fields
set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
find_package(Protobuf 3.12 REQUIRED)
if(Protobuf_VERSION VERSION_LESS "3.15")
    # Before Protobuf 3.15, proto3 optional fields need a flag of protoc, which 'protobuf_generate' of CMake passes from 3.28 only
    include("${PROJECT_SOURCE_DIR}/cmake/third-party/protobuf_generate.cmake")
    set(BF_PROTOC_OPTIONS PROTOC_OPTIONS --experimental_allow_proto3_optional)
endif()

# Find gRPC
find_package(gRPC QUIET)
if (NOT gRPC_FOUND)
    # Fallback to custom gRPC find for distributions without the CMake package of gRPC
    list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/third-party")
endif()
find_package(gRPC REQUIRED)

# Create library
file(GLOB_RECURSE PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto)
add_library(${PROJECT_NAME}-protocol ${PROTO_FILES})
target_include_directories (${PROJECT_NAME}-protocol PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}-protocol PUBLIC gRPC::grpc++ protobuf::libprotobuf)
protobuf_generate(TARGET ${PROJECT_NAME}-protocol LANGUAGE cpp ${BF_PROTOC_OPTIONS})
get_target_property(gRPC_CPP_PLUGIN gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ${PROJECT_NAME}-protocol LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${gRPC_CPP_PLUGIN}" ${BF_PROTOC_OPTIONS})

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