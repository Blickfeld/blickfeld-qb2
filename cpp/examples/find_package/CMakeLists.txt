if (NOT TARGET blickfeld-qb2)
    find_package(blickfeld-qb2 REQUIRED)
endif()

add_executable(find_package-example main.cpp)
target_link_libraries(find_package-example PUBLIC blickfeld-qb2)