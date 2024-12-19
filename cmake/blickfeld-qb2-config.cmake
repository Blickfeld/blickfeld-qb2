include(CMakeFindDependencyMacro)

if(NOT blickfeld-qb2_FOUND)
    # Protocol: Find Protobuf
    find_package(Protobuf 3.12.4)
    if(NOT Protobuf_FOUND)
        find_dependency(Protobuf 3.6.1)
        include("${CMAKE_CURRENT_LIST_DIR}/third-party/protobuf_generate.cmake")
    endif()

    # Protocol: Find gRPC
    find_dependency(gRPC QUIET)
    if (NOT gRPC_FOUND)
        # Fallback to custom gRPC find (due to Ubuntu 20.04)
        list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third-party")
    endif()
    find_dependency(gRPC REQUIRED)

    include("${CMAKE_CURRENT_LIST_DIR}/blickfeld-qb2.cmake")

    # Print info
    message(STATUS "Loaded blickfeld-qb2 ${blickfeld-qb2_VERSION}")

    set(blickfeld-qb2_FOUND True)
endif()