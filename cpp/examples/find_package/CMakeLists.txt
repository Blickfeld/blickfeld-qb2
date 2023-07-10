find_package(blickfeld-qb2 REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example PUBLIC blickfeld-qb2)