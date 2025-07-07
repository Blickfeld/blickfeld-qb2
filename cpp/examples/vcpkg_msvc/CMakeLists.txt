cmake_minimum_required(VERSION 3.16)

if (NOT TARGET blickfeld-qb2)
    find_package(blickfeld-qb2 REQUIRED)
endif()

add_executable(vcpkg_msvc-example main.cpp)
target_link_libraries(vcpkg_msvc-example PUBLIC blickfeld-qb2)