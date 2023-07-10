cmake_minimum_required(VERSION 3.16)
project(blickfeld-qb2 VERSION 1.0.0)

# Default options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Data
add_subdirectory(data)

# Protocol
add_subdirectory(protocol)

# C++
add_subdirectory(cpp)

# Main target
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME}-protocol
    ${PROJECT_NAME}-cpp
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME})

# CMake Package
add_subdirectory(cmake)