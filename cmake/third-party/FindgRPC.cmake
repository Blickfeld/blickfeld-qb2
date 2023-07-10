#
# Locate and configure the gRPC library
#
# Adds the following targets:
#
#  gRPC::grpc - gRPC library
#  gRPC::grpc++ - gRPC C++ library
#  gRPC::grpc_cpp_plugin - C++ generator plugin for Protocol Buffers
#

if (gRPC_FOUND)
  return()
endif()

# By default have GRPC_GENERATE_CPP macro pass -I to protoc
# for each directory where a proto file is referenced.
if(NOT DEFINED GRPC_GENERATE_CPP_APPEND_PATH)
  set(GRPC_GENERATE_CPP_APPEND_PATH TRUE)
endif()

# Find gRPC include directory
find_path(GRPC_INCLUDE_DIR grpc/grpc.h)
mark_as_advanced(GRPC_INCLUDE_DIR)

# Find gRPC library
find_library(GRPC_LIBRARY NAMES grpc)
mark_as_advanced(GRPC_LIBRARY)
add_library(gRPC::grpc UNKNOWN IMPORTED)
set_target_properties(gRPC::grpc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${GRPC_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES "-lpthread;-ldl"
    IMPORTED_LOCATION ${GRPC_LIBRARY}
)

# Find gRPC C++ library
find_library(GRPC_GRPC++_LIBRARY NAMES grpc++)
mark_as_advanced(GRPC_GRPC++_LIBRARY)
add_library(gRPC::grpc++ UNKNOWN IMPORTED)
set_target_properties(gRPC::grpc++ PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${GRPC_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES gRPC::grpc
    IMPORTED_LOCATION ${GRPC_GRPC++_LIBRARY}
)

# Find gRPC CPP generator
find_program(GRPC_CPP_PLUGIN NAMES grpc_cpp_plugin)
mark_as_advanced(GRPC_CPP_PLUGIN)
add_executable(gRPC::grpc_cpp_plugin IMPORTED)
set_target_properties(gRPC::grpc_cpp_plugin PROPERTIES
    IMPORTED_LOCATION ${GRPC_CPP_PLUGIN}
)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gRPC DEFAULT_MSG
    GRPC_LIBRARY GRPC_INCLUDE_DIR GRPC_CPP_PLUGIN)

if (NOT CMAKE_CROSSCOMPILING AND gRPC_FOUND)
  # Detect version. Source: https://github.com/googleapis/google-cloud-cpp/blob/main/cmake/FindgRPC.cmake
  file(
      WRITE "${CMAKE_BINARY_DIR}/get_gRPC_version.cc"
      [====[
  #include <grpcpp/grpcpp.h>
  #include <iostream>
  int main() {
    std::cout << grpc::Version(); // no newline to simplify CMake module
    return 0;
  }
          ]====])

  try_run(
      _gRPC_GET_VERSION_STATUS
      _gRPC_GET_VERSION_COMPILE_STATUS
      "${CMAKE_BINARY_DIR}"
      "${CMAKE_BINARY_DIR}/get_gRPC_version.cc"
      LINK_LIBRARIES
      gRPC::grpc++
      gRPC::grpc
      COMPILE_OUTPUT_VARIABLE _gRPC_GET_VERSION_COMPILE_OUTPUT
      RUN_OUTPUT_VARIABLE gRPC_VERSION)

  file(REMOVE "${CMAKE_BINARY_DIR}/get_gRPC_version.cc")
endif()
