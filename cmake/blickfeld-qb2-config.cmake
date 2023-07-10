include(CMakeFindDependencyMacro)

if(NOT blickfeld-qb2_FOUND)
    # Add custom finds
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third-party")

    # Protocol: Find Protobuf
    find_package(Protobuf 3.12.4)
    if(NOT Protobuf_FOUND)
        find_dependency(Protobuf 3.6.1)
        include("${CMAKE_CURRENT_LIST_DIR}/third-party/protobuf_generate.cmake")
    endif()

    # Protocol: Find gRPC
    find_dependency(gRPC)

    include("${CMAKE_CURRENT_LIST_DIR}/blickfeld-qb2.cmake")

    # Print info
    message(STATUS "Loaded blickfeld-qb2 ${blickfeld-qb2_VERSION}")

    set(blickfeld-qb2_FOUND True)
endif()