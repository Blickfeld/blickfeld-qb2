include(CMakeFindDependencyMacro)

if(NOT blickfeld-qb2_FOUND)
    # Protocol: Find Protobuf
    find_dependency(Protobuf 3.12)

    # Protocol: Find gRPC
    find_dependency(gRPC QUIET)
    if (NOT gRPC_FOUND)
        # Fallback to custom gRPC find for distributions without the CMake package of gRPC
        list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third-party")
    endif()
    find_dependency(gRPC REQUIRED)

    include("${CMAKE_CURRENT_LIST_DIR}/blickfeld-qb2.cmake")

    # Print info
    message(STATUS "Loaded blickfeld-qb2 ${blickfeld-qb2_VERSION}")

    set(blickfeld-qb2_FOUND True)
endif()